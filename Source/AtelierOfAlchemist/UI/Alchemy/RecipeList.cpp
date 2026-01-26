#include "RecipeList.h"
#include "RecipeListSlot.h"
#include "IngredientSlot.h"
#include "IngredientSelectWidget.h"
#include "../../AoAGameInstance.h"
#include "../../InventoryManagerSubsystem.h"
#include "../../RecipeManagerSubsystem.h"
#include "../../AoAPlayerController.h"
#include "../../DataAssets/GradeHelper.h"
#include "../../UI/MyHUD.h"

#include "Components/ScrollBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void URecipeList::InitAlchemyWindow(URecipeDataAsset* InRecipeData)
{
	RecipeData = InRecipeData;

	if (Image_SelectedIcon) Image_SelectedIcon->SetVisibility(ESlateVisibility::Hidden);
	if (Text_SelectedName) Text_SelectedName->SetText(FText::GetEmpty());
	if (Button_Craft) Button_Craft->SetIsEnabled(false);

	if (Box_IngredientSlots) Box_IngredientSlots->ClearChildren();
	if (Text_ResultGrade) Text_ResultGrade->SetText(FText::GetEmpty());

	RefreshRecipeList();
}

void URecipeList::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Close)
	{
		Button_Close->OnClicked.AddDynamic(this, &URecipeList::OnCloseButtonClicked);
	}

	if (Button_Craft)
	{
		Button_Craft->OnClicked.AddDynamic(this, &URecipeList::OnCraftButtonClicked);
	}
}

void URecipeList::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsDesignTime())
	{
		if (!ScrollBox_RecipeList || !RecipeSlotClass) return;

		ScrollBox_RecipeList->ClearChildren();

		for (int32 i = 0; i < PreviewItemCount; ++i)
		{
			UUserWidget* Widget = CreateWidget<UUserWidget>(this, RecipeSlotClass);
			if (Widget)
			{
				ScrollBox_RecipeList->AddChild(Widget);
			}
		}
	}
}

void URecipeList::RefreshRecipeList()
{
	if (!ScrollBox_RecipeList || !RecipeSlotClass) return;

	ScrollBox_RecipeList->ClearChildren();

	UAoAGameInstance* GameInst = Cast<UAoAGameInstance>(GetGameInstance());
	URecipeManagerSubsystem* RecipeManager = GameInst ? GameInst->GetSubsystem<URecipeManagerSubsystem>() : nullptr;

	if (!RecipeData) return;

	for (int32 i = 0; i < RecipeData->Recipes.Num(); ++i)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(this, RecipeSlotClass);
		URecipeListSlot* NewSlot = Cast<URecipeListSlot>(Widget);

		if (!NewSlot) continue;

		const FAlchemyRecipe& Recipe = RecipeData->Recipes[i];
		bool bIsKnown = RecipeManager ? RecipeManager->IsRecipeUnlocked(Recipe.RecipeID) : false;

		NewSlot->InitSlot(Recipe, bIsKnown);
		NewSlot->OnRecipeSelected.AddUObject(this, &URecipeList::HandleRecipeSelected);

		ScrollBox_RecipeList->AddChild(NewSlot);
	}
}

void URecipeList::HandleRecipeSelected(const FAlchemyRecipe& InRecipe)
{
	SelectedRecipe = InRecipe;

	UAoAGameInstance* GameInst = Cast<UAoAGameInstance>(GetGameInstance());
	URecipeManagerSubsystem* RecipeSubsystem = GameInst->GetSubsystem<URecipeManagerSubsystem>();

	bool bIsKnown = RecipeSubsystem->IsRecipeUnlocked(InRecipe.RecipeID);

	if (bIsKnown)
	{
		if (Image_SelectedIcon)
		{
			Image_SelectedIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			UTexture2D* IconTex = InRecipe.Icon;
			if (IconTex)
			{
				Image_SelectedIcon->SetBrushFromTexture(IconTex);
			}
		}

		if (Text_SelectedName)
		{
			Text_SelectedName->SetText(InRecipe.RecipeName);
		}

		CreateIngredientSlots(InRecipe);
		UpdateCraftingState();
	}
}

void URecipeList::OnCloseButtonClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(PC->GetHUD()))
		{
			MyHUD->CloseAllWidgets();
		}
		else
		{
			RemoveFromParent();
			PC->SetInputMode(FInputModeGameOnly());
			PC->bShowMouseCursor = false;
		}
	}
}

void URecipeList::CreateIngredientSlots(const FAlchemyRecipe& Recipe)
{
	if (!Box_IngredientSlots || !IngredientSlotClass) return;

	Box_IngredientSlots->ClearChildren();
	CreatedSlots.Empty();

	for (const auto& Pair : Recipe.Ingredients)
	{
		UItemDataAsset* ItemAsset = Pair.Key;
		int32 Count = Pair.Value;

		UIngredientSlot* NewSlot = CreateWidget<UIngredientSlot>(this, IngredientSlotClass);
		if (NewSlot)
		{
			NewSlot->InitRequirement(ItemAsset, Count);

			NewSlot->OnRequestPopup.AddDynamic(this, &URecipeList::OnRequireSlotClicked);

			Box_IngredientSlots->AddChild(NewSlot);
			CreatedSlots.Add(NewSlot);
		}
	}
}

void URecipeList::OnRequireSlotClicked(UIngredientSlot* SlotWidget)
{
	if (!SlotWidget) return;

	CurrentEditingSlot = SlotWidget;
	UItemDataAsset* TargetItem = SlotWidget->GetRequiredAsset();

	UAoAGameInstance* GameInst = Cast<UAoAGameInstance>(GetGameInstance());
	UInventoryManagerSubsystem* InvSys = GameInst->GetSubsystem<UInventoryManagerSubsystem>();

	TArray<FInventorySearchResult> FoundItems = InvSys->FindItemsByAsset(TargetItem);

	if (PopupClass)
	{
		UIngredientSelectWidget* Popup = CreateWidget<UIngredientSelectWidget>(this, PopupClass);
		if (Popup)
		{
			Popup->InitPopup(FoundItems, TargetItem);
			Popup->OnIngredientPicked.AddDynamic(this, &URecipeList::OnMaterialSelectedFromPopup);
			Popup->AddToViewport(100);
		}
	}
}

void URecipeList::OnMaterialSelectedFromPopup(int32 InventoryIndex, EItemGrade Grade)
{
	if (CurrentEditingSlot)
	{
		CurrentEditingSlot->SetSelectedMaterial(InventoryIndex, Grade);
		UpdateCraftingState();
	}

	CurrentEditingSlot = nullptr;
}

void URecipeList::UpdateCraftingState()
{
	int32 TotalScore = 0;
	int32 SelectedCount = 0;
	bool bAllSelected = true;

	for (UIngredientSlot* IngSlot : CreatedSlots)
	{
		if (IngSlot && IngSlot->IsSelected())
		{
			TotalScore += AlchemyMath::GetGradeScore(IngSlot->GetSelectedGrade());
			SelectedCount++;
		}
		else
		{
			bAllSelected = false;
		}
	}

	if (bAllSelected && SelectedCount > 0)
	{
		Button_Craft->SetIsEnabled(true);

		int32 AvgScore = TotalScore / SelectedCount;
		EItemGrade ResultGrade = AlchemyMath::GetGradeFromScore(AvgScore);

		FString GradeStr = UEnum::GetDisplayValueAsText(ResultGrade).ToString();
		if (Text_ResultGrade)
		{
			Text_ResultGrade->SetText(FText::FromString(GradeStr));
		}
	}
	else
	{
		Button_Craft->SetIsEnabled(false);
		if (Text_ResultGrade)
		{
			Text_ResultGrade->SetText(FText::FromString(TEXT("재료 선택 필요")));
		}
	}
}

void URecipeList::OnCraftButtonClicked()
{
	UAoAGameInstance* GameInst = Cast<UAoAGameInstance>(GetGameInstance());
	if (!GameInst) return;

	UInventoryManagerSubsystem* InvSys = GameInst->GetSubsystem<UInventoryManagerSubsystem>();
	if (!InvSys) return;

	int32 TotalScore = 0;
	int32 Count = 0;

	for (UIngredientSlot* IngSlot : CreatedSlots)
	{
		if (IngSlot)
		{
			TotalScore += AlchemyMath::GetGradeScore(IngSlot->GetSelectedGrade());
			Count++;

			int32 SlotIndex = IngSlot->GetSelectedInventoryIndex();
			if (SlotIndex >= 0)
			{
				InvSys->RemoveItemByIndex(this, SlotIndex, IngSlot->GetRequiredCount());
			}
		}
	}

	EItemGrade FinalGrade = EItemGrade::EIG_E;
	if (Count > 0)
	{
		FinalGrade = AlchemyMath::GetGradeFromScore(TotalScore / Count);
	}

	InvSys->AddItem(this, SelectedRecipe.ResultItem, FinalGrade, SelectedRecipe.ResultCount);

	CreateIngredientSlots(SelectedRecipe);
	UpdateCraftingState();
}
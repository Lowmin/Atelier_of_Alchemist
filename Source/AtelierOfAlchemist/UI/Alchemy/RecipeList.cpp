#include "RecipeList.h"
#include "RecipeListSlot.h"
#include "IngredientSlot.h"
#include "IngredientSelectWidget.h"
#include "../../AoAGameInstance.h"
#include "../../InventoryManagerSubsystem.h"
#include "../../RecipeManagerSubsystem.h"
#include "../../AoAPlayerController.h"
#include "../../DataAssets/GradeHelper.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
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

void URecipeList::RefreshRecipeList()
{
	if (!GridPanel_RecipeList || !RecipeSlotClass) return;

	GridPanel_RecipeList->ClearChildren();

	UAoAGameInstance* GameInst = Cast<UAoAGameInstance>(GetGameInstance());
	URecipeManagerSubsystem* RecipeManager = GameInst ? GameInst->GetSubsystem<URecipeManagerSubsystem>() : nullptr;

	for (int32 i = 0; i < TotalSlots; ++i)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(this, RecipeSlotClass);
		URecipeListSlot* NewSlot = Cast<URecipeListSlot>(Widget);

		if (!NewSlot) continue;

		int32 Row = i / MaxColumns;
		int32 Col = i % MaxColumns;

		if (RecipeData && i < RecipeData->Recipes.Num())
		{
			const FAlchemyRecipe& Recipe = RecipeData->Recipes[i];
			bool bIsKnown = RecipeManager ? RecipeManager->IsRecipeUnlocked(Recipe.RecipeID) : false;

			NewSlot->InitSlot(Recipe, bIsKnown);
			NewSlot->OnRecipeSelected.AddUObject(this, &URecipeList::HandleRecipeSelected);
		}
		else
		{
			NewSlot->InitEmpty();
		}

		UUniformGridSlot* GridSlot = GridPanel_RecipeList->AddChildToUniformGrid(NewSlot, Row, Col);

		if (GridSlot)
		{
			GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		}
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
	RemoveFromParent();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetOwningPlayer()))
	{
		AAoAPlayerController* PC = Cast<AAoAPlayerController>(PlayerController);
		if (PC) PC->SetMenuState(false);
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
		}
	}

	EItemGrade FinalGrade = EItemGrade::EIG_E;
	if (Count > 0)
	{
		FinalGrade = AlchemyMath::GetGradeFromScore(TotalScore / Count);
	}

	InvSys->AddItem(this, SelectedRecipe.ResultItem, FinalGrade, SelectedRecipe.ResultCount);

	// 3. 재료 소모 로직 (생략 - 필요시 InvSys->RemoveItem 구현)
	// for (UIngredientSlot* IngSlot : CreatedSlots) { ... }

	CreateIngredientSlots(SelectedRecipe);
	UpdateCraftingState();

	UE_LOG(LogTemp, Log, TEXT("Craft Complete. Grade: %d"), (int32)FinalGrade);
}
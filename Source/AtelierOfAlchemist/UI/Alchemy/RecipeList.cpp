#include "RecipeList.h"
#include "RecipeListSlot.h"
#include "IngredientSlot.h"
#include "../../AoAGameInstance.h"
#include "../../InventoryManagerSubsystem.h"
#include "../../RecipeManagerSubsystem.h"
#include "../../AoAPlayerController.h"
#include "../../DataAssets/GradeHelper.h"
#include "../../UI/MyHUD.h"
#include "../Inventory/Inventory.h"

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

	bIsFocusable = true;

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
		if (ScrollBox_RecipeList && RecipeSlotClass)
		{
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

		if (Box_IngredientSlots && IngredientSlotClass)
		{
			Box_IngredientSlots->ClearChildren();
			for (int32 i = 0; i < PreviewIngredientCount; ++i)
			{
				UIngredientSlot* Widget = CreateWidget<UIngredientSlot>(this, IngredientSlotClass);
				if (Widget)
				{
					Widget->InitRequirement(nullptr, 1);
					Box_IngredientSlots->AddChild(Widget);
				}
			}
		}
	}
}

void URecipeList::RefreshRecipeList()
{
	if (!ScrollBox_RecipeList || !RecipeSlotClass) return;

	ScrollBox_RecipeList->ClearChildren();

	UAoAGameInstance* GameInstance = Cast<UAoAGameInstance>(GetGameInstance());
	URecipeManagerSubsystem* RecipeManager = GameInstance ? GameInstance->GetSubsystem<URecipeManagerSubsystem>() : nullptr;

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

	UAoAGameInstance* GameInstance = Cast<UAoAGameInstance>(GetGameInstance());
	URecipeManagerSubsystem* RecipeSubsystem = GameInstance->GetSubsystem<URecipeManagerSubsystem>();

	bool bIsKnown = RecipeSubsystem->IsRecipeUnlocked(InRecipe.RecipeID);

	if (bIsKnown)
	{
		if (Image_SelectedIcon)
		{
			Image_SelectedIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (InRecipe.Icon)
			{
				Image_SelectedIcon->SetBrushFromTexture(InRecipe.Icon);
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
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(PlayerController->GetHUD()))
		{
			MyHUD->CloseWidget(EWidgetType::Recipe);
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

	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(PlayerController->GetHUD()))
		{
			MyHUD->OpenInventoryForIngredient(TargetItem);

			if (UInventory* InventoryWidget = Cast<UInventory>(MyHUD->GetWidget(EWidgetType::Inventory)))
			{
				InventoryWidget->OnItemSelected.RemoveDynamic(this, &URecipeList::OnIngredientPickedFromInventory);
				InventoryWidget->OnItemSelected.AddDynamic(this, &URecipeList::OnIngredientPickedFromInventory);
			}
		}
	}
}

void URecipeList::OnIngredientPickedFromInventory(int32 SlotIndex)
{
	UAoAGameInstance* GameInstance = Cast<UAoAGameInstance>(GetGameInstance());
	UInventoryManagerSubsystem* InventoryManager = GameInstance ? GameInstance->GetSubsystem<UInventoryManagerSubsystem>() : nullptr;

	if (InventoryManager && CurrentEditingSlot)
	{
		const TArray<FInventorySlotStruct>& Slots = InventoryManager->GetInventorySlot();

		if (Slots.IsValidIndex(SlotIndex))
		{
			const FInventorySlotStruct& SlotData = Slots[SlotIndex];

			if (SlotData.ItemData == CurrentEditingSlot->GetRequiredAsset())
			{
				CurrentEditingSlot->SetSelectedMaterial(SlotIndex, SlotData.Grade);
				UpdateCraftingState();
			}
		}
	}

	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(PlayerController->GetHUD()))
		{
			if (UInventory* InventoryWidget = Cast<UInventory>(MyHUD->GetWidget(EWidgetType::Inventory)))
			{
				InventoryWidget->OnItemSelected.RemoveDynamic(this, &URecipeList::OnIngredientPickedFromInventory);
			}
		}
	}

	CurrentEditingSlot = nullptr;
}

void URecipeList::UpdateCraftingState()
{
	int32 TotalScore = 0;
	int32 SelectedCount = 0;
	bool bAllSelected = true;

	for (UIngredientSlot* IngredientSlot : CreatedSlots)
	{
		if (IngredientSlot && IngredientSlot->IsSelected())
		{
			TotalScore += AlchemyMath::GetGradeScore(IngredientSlot->GetSelectedGrade());
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
	UAoAGameInstance* GameInstance = Cast<UAoAGameInstance>(GetGameInstance());
	if (!GameInstance) return;

	UInventoryManagerSubsystem* InventoryManager = GameInstance->GetSubsystem<UInventoryManagerSubsystem>();
	if (!InventoryManager) return;

	int32 TotalScore = 0;
	int32 Count = 0;

	for (UIngredientSlot* IngredientSlot : CreatedSlots)
	{
		if (IngredientSlot)
		{
			TotalScore += AlchemyMath::GetGradeScore(IngredientSlot->GetSelectedGrade());
			Count++;

			int32 SlotIndex = IngredientSlot->GetSelectedInventoryIndex();
			if (SlotIndex >= 0)
			{
				InventoryManager->RemoveItemByIndex(this, SlotIndex, IngredientSlot->GetRequiredCount());
			}
		}
	}

	EItemGrade FinalGrade = EItemGrade::EIG_E;
	if (Count > 0)
	{
		FinalGrade = AlchemyMath::GetGradeFromScore(TotalScore / Count);
	}

	InventoryManager->AddItem(this, SelectedRecipe.ResultItem, FinalGrade, SelectedRecipe.ResultCount);
	CreateIngredientSlots(SelectedRecipe);
	UpdateCraftingState();
}
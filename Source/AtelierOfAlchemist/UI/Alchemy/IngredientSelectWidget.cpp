#include "IngredientSelectWidget.h"
#include "IngredientSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Button.h"

void UIngredientSelectWidget::InitPopup(const TArray<FInventorySearchResult>& FoundItems, UItemDataAsset* ItemAsset)
{
    if (!Grid_ItemList || !ItemSlotClass) return;

    Grid_ItemList->ClearChildren();

    for (int32 i = 0; i < FoundItems.Num(); ++i)
    {
        UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(this, ItemSlotClass);
        UIngredientSlot* NewSlot = Cast<UIngredientSlot>(CreatedWidget);

        if (NewSlot)
        {
            const FInventorySearchResult& Data = FoundItems[i];
            \
            NewSlot->InitIngredientSlot(Data.SlotIndex, Data.SlotData.Grade, Data.SlotData.Quantity, ItemAsset);\
            NewSlot->OnSlotSelected.AddDynamic(this, &UIngredientSelectWidget::HandleItemClicked);
            \
            Grid_ItemList->AddChildToUniformGrid(NewSlot, i / 5, i % 5);
        }
    }
}

void UIngredientSelectWidget::HandleItemClicked(int32 SlotIndex, EItemGrade Grade)
{
	if (OnIngredientPicked.IsBound())
	{
		OnIngredientPicked.Broadcast(SlotIndex, Grade);
	}

	RemoveFromParent();
}

void UIngredientSelectWidget::OnCancelClicked()
{
	RemoveFromParent();
}

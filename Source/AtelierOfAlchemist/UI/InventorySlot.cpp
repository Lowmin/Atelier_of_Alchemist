// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "Inventory/InventorySlotStruct.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../DataAssets/ItemDataAsset.h"

void UInventorySlot::UpdateSlot(const FInventorySlotStruct& SlotData)
{
	if (SlotData.ItemData.IsNull() || SlotData.Quantity == 0)
	{
		if (ItemImage)		ItemImage->SetVisibility(ESlateVisibility::Hidden);
		if (ItemQuantity)	ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
		if (ItemGrade)		ItemGrade->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UItemDataAsset* ItemDataAsset = SlotData.ItemData.LoadSynchronous();

		if (ItemImage && ItemDataAsset->ItemIcon)
		{
			ItemImage->SetBrushFromSoftTexture(ItemDataAsset->ItemIcon);
			ItemImage->SetVisibility(ESlateVisibility::Visible);
		}
		if (ItemQuantity)
		{
			ItemQuantity->SetText(FText::AsNumber(SlotData.Quantity));
			ItemQuantity->SetVisibility(ESlateVisibility::Visible);
		}
		if (ItemGrade)
		{
			FText GradeText = UEnum::GetDisplayValueAsText(SlotData.Grade);
			ItemGrade->SetText(GradeText);
			ItemGrade->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

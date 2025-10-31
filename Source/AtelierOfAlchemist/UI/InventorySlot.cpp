// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "Inventory/InventorySlotStruct.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../DataAssets/ItemDataAsset.h"
#include "Inventory/InventoryItemInfo.h"
#include "Inventory/Inventory.h"
#include "Blueprint/UserWidget.h"

void UInventorySlot::UpdateSlot(const FInventorySlotStruct& SlotData)
{
	ItemSlotData = SlotData;

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
			FText Quantity = FText::AsNumber(SlotData.Quantity);
			FFormatNamedArguments Args;
			Args.Add(TEXT("Quantity"), Quantity);
			FText QuantityFormat = FText::FromString(TEXT("x{Quantity}"));
			FText QuantityText = FText::Format(QuantityFormat, Args);

			ItemQuantity->SetText(QuantityText);
			ItemQuantity->SetVisibility(ESlateVisibility::Visible);
		}
		if (ItemGrade)
		{
			FText Grade = UEnum::GetDisplayValueAsText(SlotData.Grade);

			ItemGrade->SetText(Grade);
			ItemGrade->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInventorySlot::SetOwningInventory(UInventory* OwningInventory)
{
	if (OwningInventory)
	{
		ItemInfo = OwningInventory->GetItemInfoWidget();
	}
}

void UInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (ItemInfo)
	{
		ItemInfo->UpdateInfo(ItemSlotData);
	}
}

void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (ItemInfo)
	{
		ItemInfo->ClearInfo();
	}
}

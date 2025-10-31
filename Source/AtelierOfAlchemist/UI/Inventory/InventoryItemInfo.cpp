// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemInfo.h"
#include "InventorySlotStruct.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryItemInfo::UpdateInfo(const FInventorySlotStruct& SlotData)
{
	if (SlotData.ItemData.IsNull())
	{
		ClearInfo();
		return;
	}

	UItemDataAsset* ItemAsset = SlotData.ItemData.LoadSynchronous();
	if (ItemAsset == nullptr)
	{
		ClearInfo();
		return;
	}

	if (ItemImage)
	{
		ItemImage->SetBrushFromSoftTexture(ItemAsset->ItemIcon);
		ItemImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (ItemName)
	{
		ItemName->SetText(ItemAsset->ItemName);
		ItemName->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (ItemQuantity)
	{
		ItemQuantity->SetText(FText::AsNumber(SlotData.Quantity));
		ItemQuantity->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (ItemGrade)
	{
		FText GradeText = UEnum::GetDisplayValueAsText(SlotData.Grade);
		ItemGrade->SetText(GradeText);
		ItemGrade->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	
	if (ItemDescription)
	{
		ItemDescription->SetText(ItemAsset->ItemDescription);
		ItemDescription->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UInventoryItemInfo::ClearInfo()
{
	if (ItemImage)
	{
		ItemImage->SetBrushFromSoftTexture(nullptr);
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ItemName)
	{
		ItemName->SetText(FText());
		ItemName->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ItemQuantity)
	{
		ItemQuantity->SetText(FText());
		ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ItemGrade)
	{
		ItemGrade->SetText(FText());
		ItemGrade->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ItemDescription)
	{
		ItemDescription->SetText(FText());
		ItemDescription->SetVisibility(ESlateVisibility::Hidden);
	}
}

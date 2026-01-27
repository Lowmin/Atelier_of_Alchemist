// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemInfo.h"
#include "../../InventoryManagerSubsystem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"

void UInventoryItemInfo::UpdateInfo(const FInventorySlotStruct& SlotData)
{
	if (!SlotData.ItemData)
	{
		ClearInfo();
		return;
	}

	UItemDataAsset* ItemAsset = SlotData.ItemData;
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

	if (ItemDataPanel)
	{
		ItemDataPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (Text_Type)
	{
		Text_Type->SetText(UEnum::GetDisplayValueAsText(ItemAsset->ItemType));
	}

	if (ItemAsset->ItemType != EItemType::EIT_Equip) 
	{
		EquipStatPanel->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	else
	{
		EquipStatPanel->SetVisibility(ESlateVisibility::Visible);

		if (Text_EquipParts) Text_EquipParts->SetText(UEnum::GetDisplayValueAsText(ItemAsset->Part));
		if (Text_HP) Text_HP->SetText(FText::AsNumber(ItemAsset->EquipMaxHealth));
		if (Text_Atk) Text_Atk->SetText(FText::AsNumber(ItemAsset->EquipAttackPower));
		if (Text_Def) Text_Def->SetText(FText::AsNumber(ItemAsset->EquipDefense));
		if (Text_Spd) Text_Spd->SetText(FText::AsNumber(ItemAsset->EquipSpeed));
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

	if (ItemDataPanel)
	{
		ItemDataPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

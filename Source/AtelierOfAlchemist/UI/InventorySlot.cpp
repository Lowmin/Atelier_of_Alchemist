// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "Inventory/InventorySlotStruct.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../DataAssets/ItemDataAsset.h"
#include "Inventory/InventoryItemInfo.h"
#include "Inventory/Inventory.h"
#include "Blueprint/UserWidget.h"

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_ItemSlot)
	{
		Button_ItemSlot->OnClicked.AddDynamic(this, &UInventorySlot::OnButtonClicked);
	}
}

void UInventorySlot::UpdateSlot(const FInventorySlotStruct& SlotData, int32 InIndex)
{
	ItemSlotData = SlotData;
	MyIndex = InIndex;

	if (SlotData.ItemData.IsNull() || SlotData.Quantity == 0)
	{
		// 아이템이 없을 때는 숨김 (Hidden은 아예 렌더링도 안 하고 공간도 차지하지 않거나 레이아웃에 따라 다름)
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
			// [수정 핵심] Visible -> HitTestInvisible
			// 이미지는 보이되, 마우스 클릭은 통과시켜 뒤의 버튼이 눌리게 합니다.
			ItemImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (ItemQuantity)
		{
			FText Quantity = FText::AsNumber(SlotData.Quantity);
			FFormatNamedArguments Args;
			Args.Add(TEXT("Quantity"), Quantity);
			FText QuantityFormat = FText::FromString(TEXT("x{Quantity}"));
			FText QuantityText = FText::Format(QuantityFormat, Args);

			ItemQuantity->SetText(QuantityText);
			// [수정] 텍스트도 클릭을 방해하지 않도록 HitTestInvisible로 변경
			ItemQuantity->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (ItemGrade)
		{
			FText Grade = UEnum::GetDisplayValueAsText(SlotData.Grade);

			ItemGrade->SetText(Grade);
			// [수정] 등급 텍스트도 클릭 방해 금지
			ItemGrade->SetVisibility(ESlateVisibility::HitTestInvisible);
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

void UInventorySlot::OnButtonClicked()
{
	if (MyIndex >= 0)
	{
		OnSlotClicked.Broadcast(MyIndex);
	}
}

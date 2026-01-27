#include "InventorySlot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryItemInfo.h"

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Slot)
	{
		Button_Slot->OnClicked.AddDynamic(this, &UInventorySlot::OnButtonClicked);
	}
}

void UInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (SlotData.ItemData)
	{
		OnSlotHovered.Broadcast(SlotData);
	}
}

void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	FInventorySlotStruct EmptyData;
	OnSlotHovered.Broadcast(EmptyData);
}

void UInventorySlot::UpdateSlot(const FInventorySlotStruct& InSlotData, int32 InIndex)
{
	SlotData = InSlotData;
	SlotIndex = InIndex;

	if (SlotData.ItemData)
	{
		if (Image_Icon)
		{
			Image_Icon->SetBrushFromTexture(SlotData.ItemData->ItemIcon.LoadSynchronous());
			Image_Icon->SetColorAndOpacity(FLinearColor::White);
		}

		if (Text_Count)
		{
			Text_Count->SetText(FText::AsNumber(SlotData.Quantity));
			Text_Count->SetVisibility(ESlateVisibility::Visible);
		}

		if (Text_Grade)
		{
			FString GradeStr = UEnum::GetDisplayValueAsText(SlotData.Grade).ToString();
			Text_Grade->SetText(FText::FromString(GradeStr));
			Text_Grade->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (Image_Icon) Image_Icon->SetColorAndOpacity(FLinearColor::Transparent);
		if (Text_Count) Text_Count->SetVisibility(ESlateVisibility::Hidden);
		if (Text_Grade) Text_Grade->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventorySlot::InitAsUnequipSlot()
{
	SlotIndex = -1;
	SlotData = FInventorySlotStruct();

	if (Image_Icon)
	{
		if (UnequipIconTexture)
		{
			Image_Icon->SetBrushFromTexture(UnequipIconTexture);
			Image_Icon->SetColorAndOpacity(FLinearColor::White);
		}
		else
		{
			Image_Icon->SetColorAndOpacity(FLinearColor::Red);
		}
	}

	if (Text_Count) Text_Count->SetVisibility(ESlateVisibility::Hidden);

	if (Text_Grade)
	{
		Text_Grade->SetText(FText::GetEmpty());
		Text_Grade->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventorySlot::OnButtonClicked()
{
	OnSlotClicked.Broadcast(SlotIndex);
}
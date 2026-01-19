#include "Inventory.h"
#include "../../InventoryManagerSubsystem.h"
#include "Components/GridPanel.h"
#include "Components/Button.h"
#include "../InventorySlot.h"
#include "InventoryItemInfo.h"
#include "../../UI/MyHUD.h"
#include "GameFramework/PlayerController.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>();
	if (InventoryManager)
	{
		InventoryManager->OnInventoryUpdated.AddDynamic(this, &UInventory::UpdateInventory);
		UpdateInventory();
	}

	if (Button_Cancel)
	{
		Button_Cancel->OnClicked.AddDynamic(this, &UInventory::OnCloseClicked);
	}
}

void UInventory::NativeDestruct()
{
	if (InventoryManager)
	{
		InventoryManager->OnInventoryUpdated.RemoveDynamic(this, &UInventory::UpdateInventory);
	}

	Super::NativeDestruct();
}

void UInventory::RefreshInventory()
{
	UpdateInventory();
}

void UInventory::SetSelectionMode(bool bIsSelection, EEquipPart InPart)
{
	bIsSelectionMode = bIsSelection;
	FilterPart = InPart;
	UpdateInventory();
}

void UInventory::UpdateInventory()
{
	if (!InventoryGrid || !InventoryManager) return;

	InventoryGrid->ClearChildren();

	const TArray<FInventorySlotStruct>& Slots = InventoryManager->GetInventorySlot();
	const int32 Columns = 8;

	int32 VisibleIndex = 0;

	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		const FInventorySlotStruct& SlotData = Slots[i];
		bool bIsEmpty = (!SlotData.ItemData);

		if (bIsSelectionMode)
		{
			if (!bIsEmpty)
			{
				UItemDataAsset* Asset = SlotData.ItemData;

				if (!Asset || Asset->ItemType != EItemType::EIT_Equip || Asset->Part != FilterPart)
				{
					continue;
				}
			}
		}

		UInventorySlot* NewSlotWidget = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		if (NewSlotWidget)
		{
			NewSlotWidget->UpdateSlot(SlotData, i);
			NewSlotWidget->SetOwningInventory(this);
			NewSlotWidget->OnSlotClicked.AddDynamic(this, &UInventory::OnSlotClicked);

			const int32 Row = VisibleIndex / Columns;
			const int32 Column = VisibleIndex % Columns;
			InventoryGrid->AddChildToGrid(NewSlotWidget, Row, Column);

			VisibleIndex++;
		}
	}
}

void UInventory::OnSlotClicked(int32 SlotIndex)
{
	if (bIsSelectionMode)
	{
		if (InventoryManager)
		{
			const TArray<FInventorySlotStruct>& Slots = InventoryManager->GetInventorySlot();
			if (Slots.IsValidIndex(SlotIndex) && !Slots[SlotIndex].ItemData)
			{
				return;
			}
		}

		OnItemSelected.Broadcast(SlotIndex);

		if (APlayerController* PC = GetOwningPlayer())
		{
			if (AMyHUD* MyHUD = Cast<AMyHUD>(PC->GetHUD()))
			{
				MyHUD->CloseWidget(EWidgetType::Inventory);
			}
		}

		bIsSelectionMode = false;
	}
}

void UInventory::OnCloseClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(PC->GetHUD()))
		{
			MyHUD->CloseWidget(EWidgetType::Inventory);
		}
	}

	bIsSelectionMode = false;
}

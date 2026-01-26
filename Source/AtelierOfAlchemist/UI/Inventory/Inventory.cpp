#include "Inventory.h"
#include "../../InventoryManagerSubsystem.h"
#include "Components/GridPanel.h"
#include "Components/Button.h"
#include "../InventorySlot.h"
#include "InventoryItemInfo.h"
#include "Components/GridSlot.h"
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

void UInventory::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsDesignTime())
	{
		if (!InventoryGrid || !InventorySlotClass) return;

		InventoryGrid->ClearChildren();

		for (int32 R = 0; R < PreviewRows; ++R)
		{
			for (int32 C = 0; C < PreviewColumns; ++C)
			{
				UInventorySlot* PreviewSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
				if (PreviewSlot)
				{
					UGridSlot* GridSlot = InventoryGrid->AddChildToGrid(PreviewSlot, R, C);

					if (GridSlot)
					{
						GridSlot->SetPadding(SlotPadding);
						GridSlot->SetHorizontalAlignment(HAlign_Fill);
						GridSlot->SetVerticalAlignment(VAlign_Fill);
					}
				}
			}
		}
	}
}

void UInventory::RefreshInventory()
{
	UpdateInventory();
}

void UInventory::SetSelectionMode(bool bIsSelection, EEquipPart InPart, FName InCharacterID)
{
	bIsSelectionMode = bIsSelection;
	FilterPart = InPart;
	TargetCharacterID = InCharacterID;
	UpdateInventory();
}

void UInventory::UpdateInventory()
{
	if (!InventoryGrid || !InventoryManager) return;

	InventoryGrid->ClearChildren();

	const TArray<FInventorySlotStruct>& Slots = InventoryManager->GetInventorySlot();

	const int32 Columns = PreviewColumns > 0 ? PreviewColumns : 8;

	int32 VisibleIndex = 0;

	if (bIsSelectionMode)
	{
		UInventorySlot* UnequipSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		if (UnequipSlot)
		{
			UnequipSlot->InitAsUnequipSlot();
			UnequipSlot->SetOwningInventory(this);
			UnequipSlot->OnSlotClicked.AddDynamic(this, &UInventory::OnSlotClicked);
			UnequipSlot->OnSlotHovered.AddDynamic(this, &UInventory::OnItemHovered);

			UGridSlot* GridSlot = InventoryGrid->AddChildToGrid(UnequipSlot, 0, 0);
			if (GridSlot)
			{
				GridSlot->SetPadding(SlotPadding);
				GridSlot->SetHorizontalAlignment(HAlign_Fill);
				GridSlot->SetVerticalAlignment(VAlign_Fill);
			}

			VisibleIndex++;
		}
	}

	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		const FInventorySlotStruct& SlotData = Slots[i];

		if (bIsSelectionMode)
		{
			if (!SlotData.ItemData) continue;

			if (FilterPart != EEquipPart::None)
			{
				if (SlotData.ItemData->Part != FilterPart) continue;
			}
		}

		UInventorySlot* NewSlotWidget = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		if (NewSlotWidget)
		{
			NewSlotWidget->UpdateSlot(SlotData, i);
			NewSlotWidget->SetOwningInventory(this);
			NewSlotWidget->OnSlotClicked.AddDynamic(this, &UInventory::OnSlotClicked);
			NewSlotWidget->OnSlotHovered.AddDynamic(this, &UInventory::OnItemHovered);

			const int32 Row = VisibleIndex / Columns;
			const int32 Column = VisibleIndex % Columns;

			UGridSlot* GridSlot = InventoryGrid->AddChildToGrid(NewSlotWidget, Row, Column);

			if (GridSlot)
			{
				GridSlot->SetPadding(SlotPadding);

				GridSlot->SetHorizontalAlignment(HAlign_Fill);
				GridSlot->SetVerticalAlignment(VAlign_Fill);
			}

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
			if (SlotIndex == -1)
			{
				if (TargetCharacterID != NAME_None)
				{
					InventoryManager->UnequipItemFromCharacter(TargetCharacterID, FilterPart);
				}
			}
			else
			{
				const TArray<FInventorySlotStruct>& Slots = InventoryManager->GetInventorySlot();
				if (!Slots.IsValidIndex(SlotIndex) || !Slots[SlotIndex].ItemData)
				{
					return;
				}

				if (TargetCharacterID != NAME_None)
				{
					InventoryManager->EquipItemToCharacter(TargetCharacterID, SlotIndex, FilterPart);
				}
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
		TargetCharacterID = NAME_None;
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
	TargetCharacterID = NAME_None;
}

void UInventory::OnItemHovered(const FInventorySlotStruct& SlotData)
{
	if (ItemInfoWidget)
	{
		ItemInfoWidget->UpdateInfo(SlotData);
	}
}
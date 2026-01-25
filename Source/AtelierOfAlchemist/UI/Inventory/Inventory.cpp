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

	const int32 Columns = PreviewColumns > 0 ? PreviewColumns : 8;

	int32 VisibleIndex = 0;

	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		const FInventorySlotStruct& SlotData = Slots[i];

		UInventorySlot* NewSlotWidget = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		if (NewSlotWidget)
		{
			NewSlotWidget->UpdateSlot(SlotData, i);
			NewSlotWidget->SetOwningInventory(this);
			NewSlotWidget->OnSlotClicked.AddDynamic(this, &UInventory::OnSlotClicked);

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

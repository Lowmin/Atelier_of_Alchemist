// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "../../InventoryManagerSubsystem.h"
#include "Components/GridPanel.h"
#include "InventorySlotStruct.h"
#include "../InventorySlot.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>();
	if (InventoryManager)
	{
		InventoryManager->OnInventoryUpdated.AddDynamic(this, &UInventory::UpdateInventory);
		UpdateInventory();
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

void UInventory::UpdateInventory()
{
	InventoryGrid->ClearChildren();

	const TArray<FInventorySlotStruct>& Slots = InventoryManager->GetInventorySlot();

	const int32 Columns = 6;

	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		const FInventorySlotStruct& SlotData = Slots[i];

		UInventorySlot* NewSlotWidget = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		if (NewSlotWidget)
		{
			NewSlotWidget->UpdateSlot(SlotData);

			const int32 Row = i / Columns;
			const int32 Column = i % Columns;
			InventoryGrid->AddChildToGrid(NewSlotWidget, Row, Column);
		}
	}
}

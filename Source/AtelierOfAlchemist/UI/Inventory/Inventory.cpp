// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "../../InventoryManagerSubsystem.h"
#include "Components/GridPanel.h"
#include "InventorySlotStruct.h"
#include "../InventorySlot.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"

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

	const int32 Columns = 8;

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

UWidgetAnimation* UInventory::GetAnimation(FName AnimationName) const
{
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("GetAnimationByName: WidgetClass is NULL for %s"), *GetNameSafe(this));
		return nullptr;
	}

	TArray<UWidgetAnimation*> Animations = WidgetClass->Animations;
	UE_LOG(LogTemp, Log, TEXT("Searching for animation '%s' in widget %s. Found %d animations total:"),
		*AnimationName.ToString(), *GetNameSafe(this), Animations.Num());

	for (UWidgetAnimation* Animation : Animations)
	{
		if (Animation)
		{
			UE_LOG(LogTemp, Log, TEXT("  - Checking animation: %s"), *Animation->GetFName().ToString());

			if (Animation->GetFName() == AnimationName)
			{
				UE_LOG(LogTemp, Log, TEXT("    -> MATCH FOUND!"));
				return Animation;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("GetAnimationByName: Animation '%s' NOT FOUND in %s!"), *AnimationName.ToString(), *GetNameSafe(this));
	return nullptr;
}

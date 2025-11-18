// Fill out your copyright notice in the Description page of Project Settings.


#include "LootBoxObject.h"
#include "../InventoryManagerSubsystem.h"
#include "Engine/GameInstance.h"

void ALootBoxObject::Interact_Implementation(APlayerCharacter* Interactor)
{
	UInventoryManagerSubsystem* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>();

	if (InventoryManager)
	{
		if (ItemList.Num() > 0)
		{
			for (int i = ItemList.Num() - 1; i >= 0; --i)
			{
				const FLootItem& LootItem = ItemList[i];

				if (LootItem.ItemData.IsNull()) continue;

				UItemDataAsset* Item = LootItem.ItemData.LoadSynchronous();
				bool isSuccess = InventoryManager->AddItem(this, Item, LootItem.ItemGrade, LootItem.Quantity);

				if (isSuccess)
				{
					ItemList.RemoveAt(i);
				}
			}
		}
	}
}

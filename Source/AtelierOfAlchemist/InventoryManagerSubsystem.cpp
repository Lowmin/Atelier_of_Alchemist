// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerSubsystem.h"

void UInventoryManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const FString DefaultDataAssetPath = TEXT("/Game/Blueprints/Data/Etc/DA_Wood.DA_Wood");
	UItemDataAsset* DefaultItemDataAsset = LoadObject<UItemDataAsset>(nullptr, *DefaultDataAssetPath);

	if (InventorySlot.Num() == 0)
	{
		InventorySlot.SetNum(MaxInventorySlot);
	}

	AddItem(DefaultItemDataAsset, EItemGrade::EIG_A, 5);
}

bool UInventoryManagerSubsystem::AddItem(UItemDataAsset* ItemDataAsset, EItemGrade ItemGrade, int32 Amount)
{
	if (!ItemDataAsset || Amount <= 0) return false;

	int32 MaxStack = ItemDataAsset->MaxStackSize;
	int32 AddAmount = Amount;

	if (MaxStack > 1)
	{
		for (int32 i = 0; i < InventorySlot.Num(); ++i)
		{
			FInventorySlotStruct& Slot = InventorySlot[i];

			if (Slot.ItemData == ItemDataAsset && Slot.Grade == ItemGrade && Slot.Quantity < MaxStack)
			{
				// 해당 칸에 겹칠 수 있는 남은 아이템 수량을 계산
				int32 RemainQuantity = MaxStack - Slot.Quantity;
				// 이 칸에 더할 아이템 갯수를 남은 양과 더해야 할 양 둘 중 작은 수로 저장
				int32 AddThisSlot = FMath::Min(RemainQuantity, AddAmount);

				Slot.Quantity += AddThisSlot;
				AddAmount -= AddThisSlot;

				if (AddAmount == 0)
				{
					InventorySort();
					return true;
				}
			}
		}
	}
	if (AddAmount > 0)
	{
		for (int32 i = 0; i < InventorySlot.Num(); ++i)
		{
			FInventorySlotStruct& Slot = InventorySlot[i];

			if (Slot.ItemData.IsNull())
			{
				int32 AddThisSlot = FMath::Min(MaxStack, AddAmount);

				Slot.ItemData = ItemDataAsset;
				Slot.Grade = ItemGrade;
				Slot.Quantity += AddThisSlot;
				AddAmount -= AddThisSlot;

				if (AddAmount == 0)
				{
					InventorySort();
					return true;
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Inventory is Full."));
	InventorySort();

	return false;
}

void UInventoryManagerSubsystem::InventorySort()
{
	InventorySlot.Sort([](const FInventorySlotStruct& A, const FInventorySlotStruct& B)
		{
			if (A.ItemData.IsNull()) return false;
			if (B.ItemData.IsNull()) return true;

			UItemDataAsset* ItemDataA = A.ItemData.LoadSynchronous();
			UItemDataAsset* ItemDataB = B.ItemData.LoadSynchronous();

			if (ItemDataA == nullptr) return false;
			if (ItemDataB == nullptr) return true;

			if (ItemDataA->ItemName.CompareTo(ItemDataB->ItemName) != 0)
				return ItemDataA->ItemName.CompareTo(ItemDataB->ItemName) < 0;

			if (A.Grade != B.Grade)
			return static_cast<int32>(A.Grade) < static_cast<int32>(B.Grade);

			return A.Quantity > B.Quantity;
		}
	);

	OnInventoryUpdated.Broadcast();
}

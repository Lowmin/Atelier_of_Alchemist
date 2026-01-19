#include "InventoryManagerSubsystem.h"
#include "PlayerRuntimeData.h"
#include "GuildMemberManagerSubsystem.h"

void UInventoryManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InventorySlot.SetNum(MaxInventorySlot);

	for (auto& Slot : InventorySlot)
	{
		Slot.ItemData = nullptr;
		Slot.Quantity = 0;
		Slot.Grade = EItemGrade::EIG_E;
	}
}

bool UInventoryManagerSubsystem::AddItem(const UObject* WorldContextObject, UItemDataAsset* ItemDataAsset, EItemGrade ItemGrade, int32 Amount)
{
	if (!ItemDataAsset) return false;

	for (int32 i = 0; i < InventorySlot.Num(); ++i)
	{
		if (InventorySlot[i].ItemData == ItemDataAsset &&
			InventorySlot[i].Grade == ItemGrade &&
			InventorySlot[i].Quantity < ItemDataAsset->MaxStackSize)
		{
			int32 SpaceRemaining = ItemDataAsset->MaxStackSize - InventorySlot[i].Quantity;
			int32 AddAmount = FMath::Min(SpaceRemaining, Amount);

			InventorySlot[i].Quantity += AddAmount;
			Amount -= AddAmount;

			if (Amount <= 0)
			{
				OnInventoryUpdated.Broadcast();
				return true;
			}
		}
	}

	for (int32 i = 0; i < InventorySlot.Num(); ++i)
	{
		if (InventorySlot[i].Quantity == 0)
		{
			InventorySlot[i].ItemData = ItemDataAsset;
			InventorySlot[i].Grade = ItemGrade;
			InventorySlot[i].Quantity = Amount;

			OnItemAdded.Broadcast(ItemDataAsset, ItemGrade, Amount);
			OnInventoryUpdated.Broadcast();
			return true;
		}
	}

	return false;
}

bool UInventoryManagerSubsystem::RemoveItemByIndex(const UObject* WorldContextObject, int32 SlotIndex, int32 Amount)
{
	if (!InventorySlot.IsValidIndex(SlotIndex)) return false;

	if (InventorySlot[SlotIndex].Quantity < Amount) return false;

	InventorySlot[SlotIndex].Quantity -= Amount;

	if (InventorySlot[SlotIndex].Quantity <= 0)
	{
		InventorySlot[SlotIndex] = FInventorySlotStruct();
	}

	OnInventoryUpdated.Broadcast();
	return true;
}

TArray<FInventorySearchResult> UInventoryManagerSubsystem::FindItemsByAsset(UItemDataAsset* TargetAsset)
{
	TArray<FInventorySearchResult> Results;

	for (int32 i = 0; i < InventorySlot.Num(); ++i)
	{
		if (InventorySlot[i].Quantity > 0 && InventorySlot[i].ItemData == TargetAsset)
		{
			FInventorySearchResult Result;
			Result.SlotIndex = i;
			Result.SlotData = InventorySlot[i];
			Results.Add(Result);
		}
	}
	return Results;
}

void UInventoryManagerSubsystem::EquipItemToCharacter(FName CharacterID, int32 InvSlotIndex, EEquipPart TargetPart)
{
	if (!InventorySlot.IsValidIndex(InvSlotIndex)) return;

	UGuildMemberManagerSubsystem* GuildManager = GetGameInstance()->GetSubsystem<UGuildMemberManagerSubsystem>();
	if (!GuildManager) return;

	UPlayerRuntimeData* CharacterData = GuildManager->GetPlayerRuntimeData(CharacterID);
	if (!CharacterData) return;

	FInventorySlotStruct NewItemSlot = InventorySlot[InvSlotIndex];
	UItemDataAsset* NewItemAsset = NewItemSlot.ItemData;

	if (!NewItemAsset || NewItemAsset->Part != TargetPart) return;

	UItemDataAsset* OldItem = CharacterData->GetEquipItem(TargetPart);

	CharacterData->SetEquipItem(TargetPart, NewItemAsset);

	if (OldItem)
	{
		InventorySlot[InvSlotIndex].ItemData = OldItem;
		InventorySlot[InvSlotIndex].Quantity = 1;
		InventorySlot[InvSlotIndex].Grade = NewItemSlot.Grade;
	}
	else
	{
		InventorySlot[InvSlotIndex] = FInventorySlotStruct();
	}

	OnInventoryUpdated.Broadcast();
}

int32 UInventoryManagerSubsystem::GetItemCount(UItemDataAsset* TargetItemAsset) const
{
	if (!TargetItemAsset) return 0;

	int32 TotalCount = 0;

	for (const FInventorySlotStruct& Slot : InventorySlot)
	{
		if (Slot.Quantity > 0 && Slot.ItemData == TargetItemAsset)
		{
			TotalCount += Slot.Quantity;
		}
	}

	return TotalCount;
}

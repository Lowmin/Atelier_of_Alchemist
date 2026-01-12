#include "InventoryManagerSubsystem.h"
#include "PlayerRuntimeData.h"
#include "GuildMemberManagerSubsystem.h"

void UInventoryManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InventorySlot.SetNum(MaxInventorySlot);
}

bool UInventoryManagerSubsystem::AddItem(const UObject* WorldContextObject, UItemDataAsset* ItemDataAsset, EItemGrade ItemGrade, int32 Amount)
{
	if (!ItemDataAsset) return false;

	for (int32 i = 0; i < InventorySlot.Num(); ++i)
	{
		if (InventorySlot[i].ItemData.LoadSynchronous() == ItemDataAsset &&
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

			OnInventoryUpdated.Broadcast();
			return true;
		}
	}

	return false;
}

TArray<FInventorySearchResult> UInventoryManagerSubsystem::FindItemsByAsset(UItemDataAsset* TargetAsset)
{
	TArray<FInventorySearchResult> Results;

	for (int32 i = 0; i < InventorySlot.Num(); ++i)
	{
		if (InventorySlot[i].Quantity > 0 && InventorySlot[i].ItemData.LoadSynchronous() == TargetAsset)
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
	UItemDataAsset* NewItemAsset = NewItemSlot.ItemData.LoadSynchronous();

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
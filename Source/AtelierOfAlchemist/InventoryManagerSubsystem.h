#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataAssets/ItemDataAsset.h"
#include "UI/Inventory/InventorySlotStruct.h"
#include "InventoryManagerSubsystem.generated.h"

class UItemDataAsset;

USTRUCT(BlueprintType)
struct FInventorySearchResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex;

	UPROPERTY(BlueprintReadOnly)
	FInventorySlotStruct SlotData;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS()
class ATELIEROFALCHEMIST_API UInventoryManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	bool AddItem(const UObject* WorldContextObject, UItemDataAsset* ItemDataAsset, EItemGrade ItemGrade, int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	bool RemoveItemByIndex(const UObject* WorldContextObject, int32 SlotIndex, int32 Amount = 1);

	const TArray<FInventorySlotStruct>& GetInventorySlot() const { return InventorySlot; };

	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FInventorySearchResult> FindItemsByAsset(UItemDataAsset* TargetAsset);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EquipItemToCharacter(FName CharacterID, int32 InvSlotIndex, EEquipPart TargetPart);

protected:
	UPROPERTY(SaveGame)
	TArray<FInventorySlotStruct> InventorySlot;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxInventorySlot = 32;
};
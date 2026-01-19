#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataAssets/ItemDataAsset.h"
#include "InventoryManagerSubsystem.generated.h"

class UItemDataAsset;

USTRUCT(BlueprintType)
struct FInventorySlotStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemDataAsset* ItemData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemGrade Grade = EItemGrade::EIG_E;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 0;
};

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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemAdded, UItemDataAsset*, ItemData, EItemGrade, Grade, int32, Amount);

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

	UPROPERTY(BlueprintAssignable)
	FOnItemAdded OnItemAdded;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FInventorySearchResult> FindItemsByAsset(UItemDataAsset* TargetAsset);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EquipItemToCharacter(FName CharacterID, int32 InvSlotIndex, EEquipPart TargetPart);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetItemCount(UItemDataAsset* TargetItemAsset) const;

protected:
	UPROPERTY(SaveGame)
	TArray<FInventorySlotStruct> InventorySlot;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxInventorySlot = 32;
};

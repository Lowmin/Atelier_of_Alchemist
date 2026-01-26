#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataAssets/ItemDataAsset.h"
#include "InventoryManagerSubsystem.generated.h"

class UPlayerRuntimeData;

USTRUCT(BlueprintType)
struct FInventorySlotStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemDataAsset* ItemData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemGrade Grade = EItemGrade::EIG_E;
};

USTRUCT(BlueprintType)
struct FInventorySearchResult
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex = -1;

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

	UFUNCTION(BlueprintCallable)
	bool AddItem(const UObject* WorldContextObject, UItemDataAsset* ItemDataAsset, EItemGrade ItemGrade, int32 Amount);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemByIndex(const UObject* WorldContextObject, int32 SlotIndex, int32 Amount);

	UFUNCTION(BlueprintCallable)
	void EquipItemToCharacter(FName CharacterID, int32 InvSlotIndex, EEquipPart TargetPart);

	UFUNCTION(BlueprintCallable)
	void UnequipItemFromCharacter(FName CharacterID, EEquipPart TargetPart);

	UFUNCTION(BlueprintCallable)
	const TArray<FInventorySlotStruct>& GetInventorySlot() const { return InventorySlot; }

	UFUNCTION(BlueprintCallable)
	TArray<FInventorySearchResult> FindItemsByAsset(UItemDataAsset* TargetAsset);

	UFUNCTION(BlueprintCallable)
	int32 GetItemCount(UItemDataAsset* TargetItemAsset) const;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnItemAdded OnItemAdded;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventorySlotStruct> InventorySlot;

	const int32 MaxInventorySlot = 30;
};
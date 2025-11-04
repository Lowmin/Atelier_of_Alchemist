// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataAssets/ItemDataAsset.h"
#include "UI/Inventory/InventorySlotStruct.h"
#include "InventoryManagerSubsystem.generated.h"

class UItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
/**
 *
 */
UCLASS()
class ATELIEROFALCHEMIST_API UInventoryManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UItemDataAsset* ItemDataAsset, EItemGrade ItemGrade, int32 Amount = 1);

	const TArray<FInventorySlotStruct>& GetInventorySlot() const { return InventorySlot; };

	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;
protected:
	UPROPERTY(SaveGame)
	TArray<FInventorySlotStruct> InventorySlot;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxInventorySlot = 32;

	void InventorySort();
};

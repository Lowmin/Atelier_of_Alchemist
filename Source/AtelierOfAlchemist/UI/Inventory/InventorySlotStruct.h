// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../DataAssets/ItemDataAsset.h"
#include "InventorySlotStruct.generated.h"

class UItemDataAsset;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventorySlotStruct
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TSoftObjectPtr<UItemDataAsset> ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
	EItemGrade Grade;

	FInventorySlotStruct()
		: ItemData(nullptr),
		Quantity(0),
		Grade(EItemGrade::EIG_E)
	{
	}
};

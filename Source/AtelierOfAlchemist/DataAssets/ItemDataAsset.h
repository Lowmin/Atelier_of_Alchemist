// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

UENUM(BlueprintType)
enum class EItemAttribute : uint8
{
	EIA_None	UMETA(DisplayName = "속성 없음"),
	EIA_Fire	UMETA(DisplayName = "불"),
	EIA_Water	UMETA(DisplayName = "물"),
	EIA_Wind	UMETA(DisplayName = "바람"),
	EIA_Earth	UMETA(DisplayName = "땅")
};
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Item", GetFName());
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	EItemAttribute ItemAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 MaxStackSize = 99;
};

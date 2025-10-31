// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RecipeDataAsset.generated.h"

class UItemDataAsset;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API URecipeDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Recipe", GetFName());
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	TSoftObjectPtr<UItemDataAsset> BaseItem;

	
};

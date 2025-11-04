// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeManagerSubsystem.h"
#include "DataAssets/RecipeDataAsset.h"

void URecipeManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

bool URecipeManagerSubsystem::AddRecipe(URecipeDataAsset* RecipeDataAsset)
{
	if (!RecipeDataAsset) return false;

	TSoftObjectPtr<URecipeDataAsset> RecipeDataPtr = RecipeDataPtr;

	if (AvailableRecipeList.AddUnique(RecipeDataPtr) != INDEX_NONE)
	{
		return true;
	}

	return false;
}

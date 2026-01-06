// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeManagerSubsystem.h"
#include "DataAssets/RecipeDataAsset.h"
#include "Engine/AssetManager.h"

void URecipeManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

bool URecipeManagerSubsystem::AddRecipe(FName RecipeID)
{
	if (RecipeID.IsNone()) return false;

	if (UnlockedRecipeIDs.Contains(RecipeID))
	{
		return false;
	}

	UnlockedRecipeIDs.Add(RecipeID);
	UE_LOG(LogTemp, Log, TEXT("Recipe Unlocked: %s"), *RecipeID.ToString());
	return true;
}

bool URecipeManagerSubsystem::IsRecipeUnlocked(FName RecipeID) const
{
	if (RecipeID.IsNone()) return false;
	return UnlockedRecipeIDs.Contains(RecipeID);
}
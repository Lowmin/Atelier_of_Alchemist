// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeManagerSubsystem.h"
#include "DataAssets/RecipeDataAsset.h"
#include "Engine/AssetManager.h"

void URecipeManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

bool URecipeManagerSubsystem::AddRecipe(URecipeDataAsset* RecipeDataAsset)
{
	if (!RecipeDataAsset) return false;

	TSoftObjectPtr<URecipeDataAsset> RecipeDataPtr = RecipeDataAsset;

	if (UnlockedRecipeList.Contains(RecipeDataPtr))
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRecipe: Already learned %s"), *RecipeDataAsset->GetName());
		return false;
	}

	UnlockedRecipeList.Add(RecipeDataPtr);

	UE_LOG(LogTemp, Warning, TEXT("AddRecipe: Success! %s"), *RecipeDataAsset->GetName());
	return true;
}

bool URecipeManagerSubsystem::GetAllRecipeId(TArray<FPrimaryAssetId>& RecipeId) const
{
	UAssetManager& AssetManager = UAssetManager::Get();

	const FName RecipeAssetType = FName("Recipe");

	AssetManager.GetPrimaryAssetIdList(RecipeAssetType, RecipeId);

	return RecipeId.Num() > 0;
}

TSoftObjectPtr<URecipeDataAsset> URecipeManagerSubsystem::GetRecipeFromId(const FPrimaryAssetId& RecipeId) const
{
	UAssetManager& AssetManager = UAssetManager::Get();
	FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(RecipeId);

	return TSoftObjectPtr<URecipeDataAsset>(AssetPath);
}

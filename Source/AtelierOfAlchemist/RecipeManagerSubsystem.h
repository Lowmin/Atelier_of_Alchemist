// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RecipeManagerSubsystem.generated.h"

class URecipeDataAsset;
/**
 *
 */
UCLASS()
class ATELIEROFALCHEMIST_API URecipeManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	bool AddRecipe(URecipeDataAsset* RecipeDataAsset);
	const TArray<TSoftObjectPtr<URecipeDataAsset>> GetUnlockedRecipes() { return UnlockedRecipeList; };
	bool GetAllRecipeId(TArray<FPrimaryAssetId>& RecipeId) const;

	UFUNCTION(BlueprintPure, Category = "Recipe")
	TSoftObjectPtr<URecipeDataAsset> GetRecipeFromId(const FPrimaryAssetId& RecipeId) const;


protected:
	UPROPERTY(SaveGame)
	TArray<TSoftObjectPtr<URecipeDataAsset>> UnlockedRecipeList;

};

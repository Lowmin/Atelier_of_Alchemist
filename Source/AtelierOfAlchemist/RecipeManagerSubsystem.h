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

	UFUNCTION(BlueprintCallable, Category = "Recipe")
	bool AddRecipe(FName RecipeID);

	UFUNCTION(BlueprintPure, Category = "Recipe")
	bool IsRecipeUnlocked(FName RecipeID) const;

	const TSet<FName>& GetUnlockedRecipeIDs() const { return UnlockedRecipeIDs; }

protected:
	UPROPERTY(VisibleAnywhere, Category = "Recipe")
	TSet<FName> UnlockedRecipeIDs;
};

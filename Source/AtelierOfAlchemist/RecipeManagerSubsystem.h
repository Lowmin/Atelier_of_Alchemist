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
	const TArray<TSoftObjectPtr<URecipeDataAsset>> GetUnlockedRecipes() { return AvailableRecipeList; };


protected:
	UPROPERTY(SaveGame)
	TArray<TSoftObjectPtr<URecipeDataAsset>> AvailableRecipeList;

	// 게임 진행 시 레시피 목록을 전부 불러와야하므로 TObjectPtr 사용
	UPROPERTY(Transient)
	TArray<TObjectPtr<URecipeDataAsset>> AllRecipeList;

};

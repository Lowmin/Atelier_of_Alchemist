// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RecipeDataAsset.generated.h"

class UItemDataAsset;

USTRUCT(BlueprintType)
struct FAlchemyRecipe
{
	GENERATED_BODY()

public:
	// 레시피 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	FName RecipeID;

	// 레시피 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	FText RecipeName;

	// 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	UTexture2D* Icon;

	// 필요한 재료(예시 = 약초, 2개)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	TMap<UItemDataAsset*, int32> Ingredients;

	// 결과 아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	UItemDataAsset* ResultItem;

	// 결과 아이템 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	int32 ResultCount = 1;
};
/**
 * 
 */
UCLASS(BlueprintType)
class ATELIEROFALCHEMIST_API URecipeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	TArray<FAlchemyRecipe> Recipes;
};

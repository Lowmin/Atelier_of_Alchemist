// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecipeListSlot.generated.h"

class UImage;
class UTextBlock;
class URecipeDataAsset;
/**
 *
 */
UCLASS()
class ATELIEROFALCHEMIST_API URecipeListSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> ResultItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultItemName;

	void UpdateSlot(const TSoftObjectPtr<URecipeDataAsset> RecipeDataAsset);
};

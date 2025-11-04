// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecipeList.generated.h"


class UImage;
class UTextBlock;
class UGridPanel;
class URecipeListSlot;
class URecipeManagerSubsystem;
/**
 *
 */
UCLASS()
class ATELIEROFALCHEMIST_API URecipeList : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void UpdateRecipeList();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UGridPanel> RecipeGrid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TSubclassOf<URecipeListSlot> RecipeListSlot;

	UPROPERTY()
	TObjectPtr<URecipeManagerSubsystem> RecipeManager;

public:
};

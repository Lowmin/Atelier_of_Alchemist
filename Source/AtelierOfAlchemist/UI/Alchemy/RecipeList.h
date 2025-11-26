// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecipeList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecipeSelected, TSoftObjectPtr<URecipeDataAsset>, RecipePtr);

class UImage;
class UTextBlock;
class UGridPanel;
class URecipeListSlot;
class URecipeManagerSubsystem;
class URecipeDataAsset;
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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> RecipeGrid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URecipeListSlot> RecipeListSlot;

	UPROPERTY()
	TObjectPtr<URecipeManagerSubsystem> RecipeManager;

	UFUNCTION()
	void OnSlotClicked(TSoftObjectPtr<URecipeDataAsset> RecipePtr);

public:
	void Show();
	void Hide();

	UPROPERTY(BlueprintAssignable)
	FOnRecipeSelected OnRecipeSelected;
};

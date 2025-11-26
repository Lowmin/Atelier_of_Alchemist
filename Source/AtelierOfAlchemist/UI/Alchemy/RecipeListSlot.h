// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecipeListSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecipeSlotClicked, TSoftObjectPtr<URecipeDataAsset>, RecipePtr);

class UImage;
class UTextBlock;
class URecipeDataAsset;
class UButton;
/**
 *
 */
UCLASS()
class ATELIEROFALCHEMIST_API URecipeListSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonClicked();

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SlotButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> ResultItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultItemName;

	void UpdateSlot(const TSoftObjectPtr<URecipeDataAsset>& RecipeDataAsset, bool bIsUnlocked);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UMaterialInterface> GrayscaleMaterialBase;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> GrayscaleDMI;

	TSoftObjectPtr<URecipeDataAsset> CachedRecipePtr;

	bool bIsSlotUnlocked = false;

	FOnRecipeSlotClicked OnSlotClicked;

};

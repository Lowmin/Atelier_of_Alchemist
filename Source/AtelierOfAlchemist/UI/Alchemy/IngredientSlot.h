// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../DataAssets/ItemDataAsset.h"
#include "IngredientSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIngredientSlotSelected, int32, InventoryIndex, EItemGrade, Grade);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestPopup, UIngredientSlot*, SlotWidget);

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class ATELIEROFALCHEMIST_API UIngredientSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitIngredientSlot(int32 InIndex, EItemGrade InGrade, int32 InQuantity, UItemDataAsset* InAsset);

	void InitRequirement(UItemDataAsset* InAsset, int32 InCount);

	void SetSelectedMaterial(int32 InIndex, EItemGrade InGrade);

	UItemDataAsset* GetRequiredAsset() const { return RequiredAsset; }
	EItemGrade GetSelectedGrade() const { return Grade; }
	bool IsSelected() const { return bIsSelected; }

	FOnIngredientSlotSelected OnSlotSelected;
	FOnRequestPopup OnRequestPopup;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Select;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Grade;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Quantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

private:
	int32 InventoryIndex = -1;
	EItemGrade Grade;

	UPROPERTY()
	UItemDataAsset* RequiredAsset;

	int32 RequiredCount = 0;
	bool bIsSelected = false;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventorySlotStruct.h"
#include "InventorySlot.generated.h"

class UImage;
class UTextBlock;
class UInventoryItemInfo;
class UInventory;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, int32, SlotIndex);
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateSlot(const FInventorySlotStruct& SlotData, int32 InIndex);
	void SetOwningInventory(UInventory* OwningInventory);

	UPROPERTY(BlueprintAssignable)
	FOnSlotClicked OnSlotClicked;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemGrade;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ItemSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FInventorySlotStruct ItemSlotData;

	UPROPERTY()
	TObjectPtr<UInventoryItemInfo> ItemInfo;

	int32 MyIndex = -1;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UFUNCTION()
	void OnButtonClicked();
};

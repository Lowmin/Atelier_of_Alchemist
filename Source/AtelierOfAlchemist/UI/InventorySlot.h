// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class UImage;
class UTextBlock;
class UInventoryItemInfo;
class UInventory;
struct FInventorySlotStruct;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateSlot(const FInventorySlotStruct& SlotData);
	void SetOwningInventory(UInventory* OwningInventory);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemGrade;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FInventorySlotStruct ItemSlotData;

	UPROPERTY()
	TObjectPtr<UInventoryItemInfo> ItemInfo;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};

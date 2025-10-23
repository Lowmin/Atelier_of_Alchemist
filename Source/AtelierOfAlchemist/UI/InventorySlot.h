// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class UImage;
class UTextBlock;
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

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemGrade;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;
};

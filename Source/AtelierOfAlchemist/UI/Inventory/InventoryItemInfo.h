// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "InventoryItemInfo.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UInventoryItemInfo : public UInventory
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemGrade;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;
};

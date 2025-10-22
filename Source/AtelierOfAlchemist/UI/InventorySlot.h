// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//void InitializeSlot();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemGrade;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

class UInventoryManagerSubsystem;
class UInventorySlot;
class UGridPanel;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UInventory : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> InventoryGrid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UInventorySlot> InventorySlotClass;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void UpdateInventory();

	UPROPERTY()
	TObjectPtr<UInventoryManagerSubsystem> InventoryManager;
};

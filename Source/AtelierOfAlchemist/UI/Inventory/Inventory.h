// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

class UInventoryManagerSubsystem;
class UInventorySlot;
class UGridPanel;
class UInventoryItemInfo;
class UWidgetAnimation;
/**
 *
 */
UCLASS()
class ATELIEROFALCHEMIST_API UInventory : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show();
	void Hide();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> InventoryGrid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UInventorySlot> InventorySlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryItemInfo> ItemInfoWidget;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeInAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutAnim;

	bool bIsAnimating = false;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void UpdateInventory();

	UPROPERTY()
	TObjectPtr<UInventoryManagerSubsystem> InventoryManager;

public:
	UInventoryItemInfo* GetItemInfoWidget() const { return ItemInfoWidget; }
};

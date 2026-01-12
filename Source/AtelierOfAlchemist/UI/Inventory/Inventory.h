// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../DataAssets/ItemDataAsset.h"
#include "Inventory.generated.h"

class UInventoryManagerSubsystem;
class UInventorySlot;
class UGridPanel;
class UInventoryItemInfo;
class UWidgetAnimation;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemSelected, int32, SelectedIndex);

UCLASS()
class ATELIEROFALCHEMIST_API UInventory : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show();
	void Hide();

	UFUNCTION(BlueprintCallable)
	void OpenAsSelectionMode(EEquipPart InPart);

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnInventoryItemSelected OnItemSelected;

	UInventoryItemInfo* GetItemInfoWidget() const { return ItemInfoWidget; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> InventoryGrid;

	// [추가] 닫기 버튼
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Cancel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UInventorySlot> InventorySlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryItemInfo> ItemInfoWidget;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeInAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutAnim;

	bool bIsAnimating = false;

	UFUNCTION()
	void UpdateInventory();

	UFUNCTION()
	void OnSlotClicked(int32 SlotIndex);

	// [추가] 닫기 버튼 클릭 시 실행될 함수
	UFUNCTION()
	void OnCloseClicked();

	UPROPERTY()
	TObjectPtr<UInventoryManagerSubsystem> InventoryManager;

private:
	bool bIsSelectionMode = false;
	EEquipPart FilterPart = EEquipPart::PET_Weapon;
};
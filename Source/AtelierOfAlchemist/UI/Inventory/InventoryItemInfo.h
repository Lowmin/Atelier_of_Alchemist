// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemInfo.generated.h"

class UTextBlock;
class UImage;
class UItemDataAsset;
struct FInventorySlotStruct;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UInventoryItemInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateInfo(const FInventorySlotStruct& SlotData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearInfo();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemGrade;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescription;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ItemDataPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Type;

	// 아이템 타입이 장비일 경우
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> EquipStatPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_EquipParts;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_HP;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Atk;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Def;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Spd;
};

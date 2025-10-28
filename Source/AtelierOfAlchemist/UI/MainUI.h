// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/Inventory.h"
#include "MainUI.generated.h"

class UInteractObjectWidget;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractObjectWidget> InteractObjectWidget;

	virtual void NativeConstruct() override;

public:
	void ShowInteractWidget(TSoftObjectPtr<UTexture2D> ItemIcon, const FText& ItemName);
	void HideInteractWidget();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventory> WBP_Inventory;
};

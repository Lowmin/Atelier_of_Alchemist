// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../DataAssets/ItemDataAsset.h"
#include "EquipSlotWidget.generated.h"

class UImage;
class UButton;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipSlotClicked, EEquipPart, Part);

UCLASS()
class ATELIEROFALCHEMIST_API UEquipSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnEquipSlotClicked OnSlotClicked;

	void SetPartType(EEquipPart InPart);
	void SetItemIcon(TSoftObjectPtr<UTexture2D> Icon);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Root;

	EEquipPart PartType;

private:
	UFUNCTION()
	void OnRootClicked();
};

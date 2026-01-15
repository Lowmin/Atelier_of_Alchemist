// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../InventoryManagerSubsystem.h"
#include "IngredientSelectWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIngredientPicked, int32, InventoryIndex, EItemGrade, Grade);
/**
 * 
 */
class UUniformGridPanel;
class UButton;

UCLASS()
class ATELIEROFALCHEMIST_API UIngredientSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitPopup(const TArray<FInventorySearchResult>& FoundItems, UItemDataAsset* ItemAsset);

	UPROPERTY(BlueprintAssignable)
	FOnIngredientPicked OnIngredientPicked;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Grid_ItemList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Cancel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> ItemSlotClass;

	UFUNCTION()
	void HandleItemClicked(int32 SlotIndex, EItemGrade Grade);

	UFUNCTION()
	void OnCancelClicked();
};

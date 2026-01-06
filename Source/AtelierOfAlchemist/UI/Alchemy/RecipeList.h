// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../DataAssets/RecipeDataAsset.h"
#include "RecipeList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecipeSelected, TSoftObjectPtr<URecipeDataAsset>, RecipePtr);

class UImage;
class UTextBlock;
class UButton;
class UUniformGridPanel;
class UScrollBox;
class URecipeListSlot;
class URecipeManagerSubsystem;
class URecipeDataAsset;
/**
 *
 */
UCLASS()
class ATELIEROFALCHEMIST_API URecipeList : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitAlchemyWindow(URecipeDataAsset* InRecipeData);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Close;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> GridPanel_RecipeList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Craft;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> RecipeSlotClass;
	
	// 레시피 선택 후 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_SelectedIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SelectedName;

private:
	UPROPERTY()
	URecipeDataAsset* RecipeData;

	FAlchemyRecipe SelectedRecipe;
	
	UFUNCTION()
	void OnCloseButtonClicked();

	void RefreshRecipeList();
	void HandleRecipeSelected(const FAlchemyRecipe& InRecipe);

	// GridPanel
	const int32 MaxColumns = 8;
	const int32 MaxRows = 3;
	const int32 TotalSlots = MaxColumns * MaxRows;
};

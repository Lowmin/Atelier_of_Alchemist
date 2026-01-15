// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../DataAssets/RecipeDataAsset.h"
#include "RecipeListSlot.generated.h"

class UImage;
class UTextBlock;
class URecipeDataAsset;
class UButton;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRecipeSelectedDelegate, const FAlchemyRecipe&);

UCLASS()
class ATELIEROFALCHEMIST_API URecipeListSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitSlot(const FAlchemyRecipe& Recipe, bool bIsKnown);
	void InitEmpty();

	FOnRecipeSelectedDelegate OnRecipeSelected;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_RecipeName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Select;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UTexture2D> UnknownIcon;

private:
	FAlchemyRecipe RecipeInfo;
	bool bIsUnLock;
};

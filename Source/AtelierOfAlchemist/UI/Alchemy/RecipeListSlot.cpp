// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeListSlot.h"
#include "../../DataAssets/ItemDataAsset.h"
#include "../../DataAssets/RecipeDataAsset.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void URecipeListSlot::InitSlot(const FAlchemyRecipe& Recipe, bool bIsKnown)
{
	RecipeInfo = Recipe;
	bIsUnLock = bIsKnown;
	Image_Icon->SetBrushFromTexture(Recipe.Icon);

	if (bIsKnown)
	{
		Text_RecipeName->SetText(Recipe.RecipeName);
	}
	else
	{
		Text_RecipeName->SetText(FText::FromString("?"));
		Image_Icon->SetColorAndOpacity(FLinearColor::Black);
	}
}

void URecipeListSlot::InitEmpty()
{
	RecipeInfo = FAlchemyRecipe();

	if (Image_Icon) Image_Icon->SetRenderOpacity(0.0f);

	if (Text_RecipeName) Text_RecipeName->SetText(FText::GetEmpty());

	if (Button_Select)
	{
		Button_Select->SetIsEnabled(false);
	}
}

void URecipeListSlot::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Select->OnClicked.AddDynamic(this, &URecipeListSlot::OnButtonClicked);
}

void URecipeListSlot::OnButtonClicked()
{
	if (OnRecipeSelected.IsBound())
	{
		OnRecipeSelected.Broadcast(RecipeInfo);
	}
}
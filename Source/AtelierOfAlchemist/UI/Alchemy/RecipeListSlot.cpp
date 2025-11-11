// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeListSlot.h"
#include "../../DataAssets/ItemDataAsset.h"
#include "../../DataAssets/RecipeDataAsset.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void URecipeListSlot::UpdateSlot(const TSoftObjectPtr<URecipeDataAsset> RecipeDataAsset)
{
	if (!ResultItemImage || !ResultItemName) return;
	if (RecipeDataAsset.IsNull()) return;

	URecipeDataAsset* LoadRecipeDataAsset = RecipeDataAsset.LoadSynchronous();
	if (!LoadRecipeDataAsset) return;

	TSoftObjectPtr<UItemDataAsset> ResultItemDataAsset = LoadRecipeDataAsset->ResultItem;
	if (!ResultItemDataAsset) return;

	UItemDataAsset* LoadResultItemDataAsset = ResultItemDataAsset.LoadSynchronous();
	if (!LoadResultItemDataAsset) return;

	ResultItemImage->SetBrushFromSoftTexture(LoadResultItemDataAsset->ItemIcon);
	ResultItemName->SetText(LoadResultItemDataAsset->ItemName);

	FText Quantity = FText::AsNumber(LoadRecipeDataAsset->ResultQuantity);
	FText FormatPattern = FText::FromString(TEXT("x{Quantity}"));
	FFormatNamedArguments Args;
	Args.Add(TEXT("Quantity"), Quantity);
	FText FinalQuantityText = FText::Format(FormatPattern, Args);
}

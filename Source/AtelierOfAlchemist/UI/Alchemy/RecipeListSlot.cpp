// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeListSlot.h"
#include "../../DataAssets/ItemDataAsset.h"
#include "../../DataAssets/RecipeDataAsset.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"

void URecipeListSlot::UpdateSlot(const TSoftObjectPtr<URecipeDataAsset>& RecipeDataAsset, bool bIsUnlocked)
{
	if (!ResultItemImage || !ResultItemName) return;

	if (RecipeDataAsset.IsNull())
	{
		ResultItemImage->SetVisibility(ESlateVisibility::Hidden);
		ResultItemName->SetText(FText::GetEmpty());
		return;
	}

	URecipeDataAsset* LoadedRecipe = RecipeDataAsset.LoadSynchronous();
	if (!LoadedRecipe) return;

	UItemDataAsset* LoadedItem = LoadedRecipe->ResultItem.LoadSynchronous();
	if (!LoadedItem) return;

	ResultItemName->SetText(LoadedItem->ItemName);

	UTexture2D* ItemTexture = LoadedItem->ItemIcon.LoadSynchronous();

	if (ItemTexture)
	{
		if (bIsUnlocked)
		{
			ResultItemImage->SetBrushFromTexture(ItemTexture);
			ResultItemImage->SetColorAndOpacity(FLinearColor::White);
		}
		else
		{
			if (GrayscaleMaterialBase)
			{
				if (!GrayscaleDMI)
				{
					GrayscaleDMI = UMaterialInstanceDynamic::Create(GrayscaleMaterialBase, this);
				}

				if (GrayscaleDMI)
				{
					GrayscaleDMI->SetTextureParameterValue(FName("InputTexture"), ItemTexture);
					ResultItemImage->SetBrushFromMaterial(GrayscaleDMI);
					ResultItemImage->SetColorAndOpacity(FLinearColor(0.7f, 0.7f, 0.7f, 1.0f));
				}
			}
			else
			{
				ResultItemImage->SetBrushFromTexture(ItemTexture);
				ResultItemImage->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
			}
		}

		ResultItemImage->SetVisibility(ESlateVisibility::Visible);
	}
}

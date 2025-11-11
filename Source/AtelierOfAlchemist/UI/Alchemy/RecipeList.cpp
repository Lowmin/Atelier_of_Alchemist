// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeList.h"
#include "../../RecipeManagerSubsystem.h"
#include "Components/GridPanel.h"

void URecipeList::NativeConstruct()
{
	Super::NativeConstruct();

	RecipeManager = GetGameInstance()->GetSubsystem<URecipeManagerSubsystem>();
	if (RecipeManager)
	{
		UpdateRecipeList();
	}
}

void URecipeList::NativeDestruct()
{
	Super::NativeDestruct();
}

void URecipeList::UpdateRecipeList()
{
	RecipeGrid->ClearChildren();

	TArray<FPrimaryAssetId> AllRecipeId;
	RecipeManager->GetAllRecipeId(AllRecipeId);

	const TArray<TSoftObjectPtr<URecipeDataAsset>>& UnlockedRecipe = RecipeManager->GetUnlockedRecipes();

	for (const FPrimaryAssetId& RecipeId : AllRecipeId)
	{
		TSoftObjectPtr<URecipeDataAsset> RecipePtr = RecipeManager->GetRecipeFromId(RecipeId);

		bool bIsUnlocked = UnlockedRecipe.Contains(RecipePtr);

		/*URecipeListSlot* NewSlotWidget = CreateWidget<URecipeListSlot>(this, RecipeListSlot);
		if (NewSlotWidget)
		{
			NewSlotWidget->UpdateSlot
		}*/
	}
}

void URecipeList::Show()
{
	SetVisibility(ESlateVisibility::Visible);
}

void URecipeList::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

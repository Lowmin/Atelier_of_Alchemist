// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeList.h"
#include "../../RecipeManagerSubsystem.h"
#include "../../DataAssets/RecipeDataAsset.h"
#include "RecipeListSlot.h"
#include "Components/GridPanel.h"
#include "Engine/AssetManager.h"

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
	if (!RecipeManager || !RecipeGrid || !RecipeListSlot) return;

	RecipeGrid->ClearChildren();

	TArray<FPrimaryAssetId> AllRecipeId;
	RecipeManager->GetAllRecipeId(AllRecipeId);

	const TArray<TSoftObjectPtr<URecipeDataAsset>>& UnlockedRecipePtrs = RecipeManager->GetUnlockedRecipes();

	TSet<FPrimaryAssetId> UnlockedIDs;
	UAssetManager& AssetManager = UAssetManager::Get();

	for (const TSoftObjectPtr<URecipeDataAsset>& Ptr : UnlockedRecipePtrs)
	{
		FPrimaryAssetId Id = AssetManager.GetPrimaryAssetIdForPath(Ptr.ToSoftObjectPath());
		if (Id.IsValid())
		{
			UnlockedIDs.Add(Id);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("UI Update: Total %d Recipes, Unlocked %d Recipes"), AllRecipeId.Num(), UnlockedIDs.Num());
	// -------------------------------------------------------------

	const int32 Columns = 5;
	int32 CurrentIndex = 0;

	for (const FPrimaryAssetId& RecipeId : AllRecipeId)
	{
		TSoftObjectPtr<URecipeDataAsset> RecipePtr = RecipeManager->GetRecipeFromId(RecipeId);

		bool bIsUnlocked = UnlockedIDs.Contains(RecipeId);

		URecipeListSlot* NewSlotWidget = CreateWidget<URecipeListSlot>(this, RecipeListSlot);

		if (NewSlotWidget)
		{
			NewSlotWidget->UpdateSlot(RecipePtr, bIsUnlocked);
			NewSlotWidget->OnSlotClicked.AddDynamic(this, &URecipeList::OnSlotClicked);

			int32 Row = CurrentIndex / Columns;
			int32 Column = CurrentIndex % Columns;

			RecipeGrid->AddChildToGrid(NewSlotWidget, Row, Column);

			CurrentIndex++;
		}
	}
}

void URecipeList::OnSlotClicked(TSoftObjectPtr<URecipeDataAsset> RecipePtr)
{
	OnRecipeSelected.Broadcast(RecipePtr);
}

void URecipeList::Show()
{
	UpdateRecipeList();
	SetVisibility(ESlateVisibility::Visible);
}

void URecipeList::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

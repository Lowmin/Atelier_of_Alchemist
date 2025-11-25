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

	const int32 Columns = 5; // (15는 너무 많을 수 있으니 5 정도로 조정 추천)
	int32 CurrentIndex = 0;

	for (const FPrimaryAssetId& RecipeId : AllRecipeId)
	{
		// 3. ID를 이용해 위젯 생성에 필요한 SoftPtr 생성
		TSoftObjectPtr<URecipeDataAsset> RecipePtr = RecipeManager->GetRecipeFromId(RecipeId);

		// 4. [핵심 수정] 이제 ID로 비교합니다. (정확함)
		bool bIsUnlocked = UnlockedIDs.Contains(RecipeId);

		URecipeListSlot* NewSlotWidget = CreateWidget<URecipeListSlot>(this, RecipeListSlot);

		if (NewSlotWidget)
		{
			NewSlotWidget->UpdateSlot(RecipePtr, bIsUnlocked);

			int32 Row = CurrentIndex / Columns;
			int32 Column = CurrentIndex % Columns;

			RecipeGrid->AddChildToGrid(NewSlotWidget, Row, Column);

			CurrentIndex++;
		}
	}
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "LootBoxObject.h"
#include "../InventoryManagerSubsystem.h"
#include "../RecipeManagerSubsystem.h"
#include "../DataAssets/RecipeDataAsset.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/GameInstance.h"

ALootBoxObject::ALootBoxObject()
{
	LootBoxMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LootBoxMesh"));
	LootBoxMesh->SetupAttachment(RootComponent);
	ObjectSensor->SetupAttachment(LootBoxMesh);

	if (IsOpened)
	{
		LootBoxMesh->PlayAnimation(OpenedAnimation, false);
	}
}

void ALootBoxObject::Interact_Implementation(APlayerCharacter* Interactor)
{
	UInventoryManagerSubsystem* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>();
	URecipeManagerSubsystem* RecipeManager = GetGameInstance()->GetSubsystem<URecipeManagerSubsystem>(); // ¹Ì¸® °¡Á®¿È

	if (InventoryManager && ItemList.Num() > 0)
	{
		for (int i = ItemList.Num() - 1; i >= 0; --i)
		{
			const FLootItem& LootItem = ItemList[i]; 

			if (LootItem.ItemData.IsNull()) continue;

			UItemDataAsset* Item = LootItem.ItemData.LoadSynchronous();
			if (!Item) continue;

			bool bSuccess = InventoryManager->AddItem(this, Item, LootItem.ItemGrade, LootItem.Quantity);

			if (bSuccess)
			{
				ItemList.RemoveAt(i);
			}
		}
	}

	if (!Recipe.IsNull())
	{
		if (RecipeManager)
		{
			URecipeDataAsset* LoadedRecipe = Recipe.LoadSynchronous();

			if (LoadedRecipe)
			{
				bool bLearned = RecipeManager->AddRecipe(LoadedRecipe);

				if (bLearned)
				{
					Recipe = nullptr;
				}
			}
		}
	}

	bool bIsItemEmpty = (ItemList.Num() == 0);
	bool bIsRecipeEmpty = Recipe.IsNull();

	if (bIsItemEmpty && bIsRecipeEmpty)
	{
		OnPlayerLeave_Implementation(Interactor);

		if (ObjectSensor)
		{
			ObjectSensor->DestroyComponent();
		}

	}
}
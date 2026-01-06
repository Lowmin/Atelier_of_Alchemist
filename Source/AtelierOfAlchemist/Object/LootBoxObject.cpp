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
	if (RootComponent)
	{
		LootBoxMesh->SetupAttachment(RootComponent);
	}
	else
	{
		SetRootComponent(LootBoxMesh);
	}

	if (ObjectSensor)
	{
		ObjectSensor->SetupAttachment(LootBoxMesh);
	}
}

void ALootBoxObject::Interact_Implementation(APlayerCharacter* Interactor)
{
	UInventoryManagerSubsystem* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>();
	URecipeManagerSubsystem* RecipeManager = GetGameInstance()->GetSubsystem<URecipeManagerSubsystem>();

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
			URecipeDataAsset* LoadedRecipeAsset = Recipe.LoadSynchronous();

			if (LoadedRecipeAsset)
			{
				for (const FAlchemyRecipe& RecipeInfo : LoadedRecipeAsset->Recipes)
				{
					RecipeManager->AddRecipe(RecipeInfo.RecipeID);
				}

				Recipe = nullptr;
			}
		}
	}

	if (!IsOpened)
	{
		IsOpened = true;
		if (OpenAnimation)
		{
			LootBoxMesh->PlayAnimation(OpenAnimation, false);
		}
	}

	bool bIsItemEmpty = (ItemList.Num() == 0);
	bool bIsRecipeEmpty = Recipe.IsNull();

	if (bIsItemEmpty && bIsRecipeEmpty)
	{
		if (ObjectSensor)
		{
			ObjectSensor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActorBase.h"
#include "../Object/CollectingObject.h"
#include "LootBoxObject.generated.h"

class UItemDataAsset;
class URecipeDataAsset;

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UItemDataAsset> ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemGrade ItemGrade;
};
/**
 *
 */
UCLASS()
class ATELIEROFALCHEMIST_API ALootBoxObject : public AInteractableActorBase
{
	GENERATED_BODY()

	ALootBoxObject();

	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsOpened{};

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe", SaveGame)
	TSoftObjectPtr<URecipeDataAsset> Recipe;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", SaveGame)
	TArray<FLootItem> ItemList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> LootBoxMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimationAsset> OpenAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimationAsset> OpenedAnimation;
};

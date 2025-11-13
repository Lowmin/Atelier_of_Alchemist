// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActorBase.h"
#include "../Object/CollectingObject.h"
#include "LootBoxObject.generated.h"

class UItemDataAsset;

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

	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", SaveGame)
	TArray<FLootItem> ItemList;

};

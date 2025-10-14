// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActorBase.h"
#include "CollectingObject.generated.h"

class UItemDataAsset;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API ACollectingObject : public AInteractableActorBase
{
	GENERATED_BODY()
	
protected:
	virtual void OnPlayerEnter_Implementation(APlayerCharacter* PlayerCharacter) override;
	virtual void OnPlayerLeave_Implementation(APlayerCharacter* PlayerCharacter) override;
	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	TObjectPtr<UItemDataAsset> DroppedItemAsset;

public:
	UItemDataAsset* GetItemData() const { return DroppedItemAsset; }
};

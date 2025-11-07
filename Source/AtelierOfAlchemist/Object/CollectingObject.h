// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActorBase.h"
#include "../DataAssets/ItemDataAsset.h"
#include "CollectingObject.generated.h"

class UItemDataAsset;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API ACollectingObject : public AInteractableActorBase
{
	GENERATED_BODY()

public:
	ACollectingObject();

protected:
	virtual void OnPlayerEnter_Implementation(APlayerCharacter* PlayerCharacter) override;
	virtual void OnPlayerLeave_Implementation(APlayerCharacter* PlayerCharacter) override;
	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;
	virtual FText GetInteractText_Implementation() const override;
	virtual TSoftObjectPtr<UTexture2D> GetInteractIcon_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	TMap<EItemGrade, float> GradeProbability;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	TObjectPtr<UItemDataAsset> DroppedItemAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	EItemGrade MaxGrade;

	EItemGrade RandomGrade();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	int32 Quantity = 1;

public:
	UItemDataAsset* GetItemData() const { return DroppedItemAsset; }
};

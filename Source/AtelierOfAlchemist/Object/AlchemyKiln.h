// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActorBase.h"
#include "AlchemyKiln.generated.h"

class URecipeDataAsset;
class URecipeList;

UCLASS()
class ATELIEROFALCHEMIST_API AAlchemyKiln : public AInteractableActorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alchemy")
	TObjectPtr<URecipeDataAsset> KilnRecipes;

protected:
	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;

private:
	URecipeList* CachedRecipeList;
};

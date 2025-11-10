// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActorBase.h"
#include "AlchemyKiln.generated.h"

/**
 *
 */
UCLASS()
class ATELIEROFALCHEMIST_API AAlchemyKiln : public AInteractableActorBase
{
	GENERATED_BODY()

	virtual void OnPlayerEnter_Implementation(APlayerCharacter* PlayerCharacter) override;
	virtual void OnPlayerLeave_Implementation(APlayerCharacter* PlayerCharacter) override;
	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;
};

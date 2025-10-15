// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActorBase.h"
#include "SpeedBuffObject.generated.h"

/**
 * 
 */
class UNiagaraComponent;

UCLASS()
class ATELIEROFALCHEMIST_API ASpeedBuffObject : public AInteractableActorBase
{
	GENERATED_BODY()
public:
	ASpeedBuffObject();

private:
	UPROPERTY(EditAnywhere, Category = "Effect")
	float SpeedMultiply{};

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraComponent> NiagaraEffect;

public:
	virtual void OnPlayerEnter_Implementation(APlayerCharacter* PlayerCharacter) override;
	virtual void OnPlayerLeave_Implementation(APlayerCharacter* PlayerCharacter) override;

};

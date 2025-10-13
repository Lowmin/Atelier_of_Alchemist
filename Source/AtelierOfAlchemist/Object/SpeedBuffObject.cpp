// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedBuffObject.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"

ASpeedBuffObject::ASpeedBuffObject()
{
	SpeedMultiply = 1.5f;
	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffec"));
	NiagaraEffect->SetupAttachment(Mesh);
}

void ASpeedBuffObject::OnPlayerEnter_Implementation(APlayerCharacter* PlayerCharacter)
{
	if (bIsEffect) return;

	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed *= SpeedMultiply;
	bIsEffect = true;
}

void ASpeedBuffObject::OnPlayerLeave_Implementation(APlayerCharacter* PlayerCharacter)
{
	if (!bIsEffect) return;

	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed /= SpeedMultiply;
	bIsEffect = false;
}

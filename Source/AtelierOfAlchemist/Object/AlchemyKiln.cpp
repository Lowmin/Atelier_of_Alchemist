// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyKiln.h"
#include "../Characters/Playable/PlayerCharacter.h"

void AAlchemyKiln::OnPlayerEnter_Implementation(APlayerCharacter* PlayerCharacter)
{
	Super::OnPlayerEnter_Implementation(PlayerCharacter);

	if (PlayerCharacter)
	{
		PlayerCharacter->SetInteractObject(this);
	}
}

void AAlchemyKiln::OnPlayerLeave_Implementation(APlayerCharacter* PlayerCharacter)
{
	Super::OnPlayerLeave_Implementation(PlayerCharacter);

	if (PlayerCharacter)
	{
		PlayerCharacter->ClearInteractObject(this);
	}
}

void AAlchemyKiln::Interact_Implementation(APlayerCharacter* Interactor)
{
	Super::Interact_Implementation(Interactor);

	if ()

}

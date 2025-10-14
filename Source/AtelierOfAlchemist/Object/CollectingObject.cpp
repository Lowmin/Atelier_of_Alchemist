// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectingObject.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "../DataAssets/ItemDataAsset.h"

void ACollectingObject::OnPlayerEnter_Implementation(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->SetInteractObject(this);
	}
}

void ACollectingObject::OnPlayerLeave_Implementation(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->ClearInteractObject(this);
	}
}

void ACollectingObject::Interact_Implementation(APlayerCharacter* Interactor)
{

}

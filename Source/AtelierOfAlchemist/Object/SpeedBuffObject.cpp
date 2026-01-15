// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedBuffObject.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "../AoAGameInstance.h"
#include "../GuildMemberManagerSubsystem.h"
#include "../InventoryManagerSubsystem.h"
#include "../DataAssets/ItemDataAsset.h"

ASpeedBuffObject::ASpeedBuffObject()
{
	SpeedMultiply = 1.5f;

	ObjectSensor->SetSphereRadius(600.0f);
	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffec"));
	NiagaraEffect->SetupAttachment(Mesh);
}

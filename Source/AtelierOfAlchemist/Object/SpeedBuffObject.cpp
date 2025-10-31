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

void ASpeedBuffObject::OnPlayerEnter_Implementation(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed *= SpeedMultiply;

	UGuildMemberManagerSubsystem* GuildManager = GetGameInstance()->GetSubsystem<UGuildMemberManagerSubsystem>();
	if (GuildManager)
	{
		GuildManager->ApplyDamageToAllPartyMembers(20.0f);
	}
	UInventoryManagerSubsystem* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>();
	if (InventoryManager)
	{
		InventoryManager->AddItem(ItemData, EItemGrade::EIG_B, 5);
	}
}

void ASpeedBuffObject::OnPlayerLeave_Implementation(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed /= SpeedMultiply;
}

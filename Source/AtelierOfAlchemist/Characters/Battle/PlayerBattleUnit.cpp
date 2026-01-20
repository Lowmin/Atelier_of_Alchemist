// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBattleUnit.h"
#include "../../BattleGameMode.h"

APlayerBattleUnit::APlayerBattleUnit()
{
	Type = ECharacterType::Player;
}

float APlayerBattleUnit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDodge) return 0.0f;

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void APlayerBattleUnit::Dodge()
{
	if (bIsDodge) return;


	if (DodgeMontage)
	{
		PlayAnimMontage(DodgeMontage);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Dodgemontage is Null."));
	}

}

void APlayerBattleUnit::OnAnimNotify_Dodge()
{
	bIsDodge = true;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Dodge, this, &APlayerBattleUnit::DodgeEnd, DodgeDuration, false);
}

void APlayerBattleUnit::DodgeEnd()
{
	bIsDodge = false;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StatComponent.h"
#include "../DataAssets/SkillDataAsset.h"
#include "../DataAssets/CharacterDataAsset.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::BattleAction_UseSkill(USkillDataAsset* Skill, const TArray<ACharacterBase*>& Targets)
{
	if (!Skill) return;

	if (UAnimMontage* Montage = Skill->SkillAnim.LoadSynchronous())
	{
		PlayAnimMontage(Montage);
	}

	for (ACharacterBase* Target : Targets)
	{
		switch (Skill->EffectType)
		{
		case ESkillEffectType::Damage:
			Target->GetStatComponent()->TakeDamage(Skill->Power);
			break;
		case ESkillEffectType::Heal:
			Target->GetStatComponent()->Heal(Skill->Power);
			break;
		}
	}
}

int32 ACharacterBase::GetLevel()
{
	return StatComponent ? StatComponent->GetLevel() : 1;
}

float ACharacterBase::GetCurHealth()
{
	return StatComponent ? StatComponent->GetCurrentHealth() : 0.0f;
}

float ACharacterBase::GetMaxHealth()
{
	return StatComponent ? StatComponent->GetMaxHealth() : 0.0f;
}

float ACharacterBase::GetAttackPower()
{
	return StatComponent ? StatComponent->GetAttackPower() : 0.0f;
}

float ACharacterBase::GetDefense()
{
	return StatComponent ? StatComponent->GetDefense() : 0.0f;
}

float ACharacterBase::GetSpeed()
{
	return StatComponent ? StatComponent->GetSpeed() : 0.0f;
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (StatComponent)
	{
		StatComponent->TakeDamage(DamageAmount);
	}

	return Damage;
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "StatComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
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

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (StatComponent)
	{
		StatComponent->TakeDamage(DamageAmount);
	}

	return Damage;
}

void ACharacterBase::Attack()
{
}

void ACharacterBase::Die()
{
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
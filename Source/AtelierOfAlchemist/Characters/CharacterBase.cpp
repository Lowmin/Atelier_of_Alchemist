// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::TakeDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0, MaxHealth);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Current JP: %.2f"), CurrentHealth)
		);
	}

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
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

int32 ACharacterBase::GetLevel()
{
	return Level;
}

float ACharacterBase::GetCurHealth()
{
	return CurrentHealth;
}

float ACharacterBase::GetMaxHealth()
{
	return MaxHealth;
}

float ACharacterBase::GetAttackPower()
{
	return AttackPower;
}

float ACharacterBase::GetDefense()
{
	return Defense;
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


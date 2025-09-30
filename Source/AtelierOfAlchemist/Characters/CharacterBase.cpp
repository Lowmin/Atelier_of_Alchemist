// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::TakeDamage()
{

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
	return CurHealth;
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


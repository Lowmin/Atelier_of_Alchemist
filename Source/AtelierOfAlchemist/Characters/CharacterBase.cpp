#include "CharacterBase.h"
#include "StatComponent.h"
#include "../DataAssets/SkillDataAsset.h"
#include "Kismet/GameplayStatics.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

int32 ACharacterBase::GetLevel() const
{
	return StatComponent ? StatComponent->GetLevel() : 1;
}

float ACharacterBase::GetCurHealth() const
{
	return StatComponent ? StatComponent->GetCurrentHealth() : 0.0f;
}

float ACharacterBase::GetMaxHealth() const
{
	return StatComponent ? StatComponent->GetMaxHealth() : 0.0f;
}

float ACharacterBase::GetAttackPower() const
{
	return StatComponent ? StatComponent->GetAttackPower() : 0.0f;
}

float ACharacterBase::GetDefense() const
{
	return StatComponent ? StatComponent->GetDefense() : 0.0f;
}

float ACharacterBase::GetSpeed() const
{
	return StatComponent ? StatComponent->GetSpeed() : 0.0f;
}

UCharacterDataAsset* ACharacterBase::GetCharacterData() const
{
	return StatComponent ? StatComponent->GetCharacterData() : nullptr;
}

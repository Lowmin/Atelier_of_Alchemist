#include "StatComponent.h"
#include "../PlayerRuntimeData.h"     
#include "../DataAssets/CharacterDataAsset.h" 

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	LinkedRuntimeData = nullptr;
	StaticData = nullptr;
	CurrentHealth = 0.0f;
}

void UStatComponent::Initialize(UPlayerRuntimeData* InRuntimeData)
{
	LinkedRuntimeData = InRuntimeData;
	if (LinkedRuntimeData)
	{
		StaticData = LinkedRuntimeData->GetCharacterDataAsset();
	}
}

void UStatComponent::InitializeFromEnemy(UCharacterDataAsset* InDataAsset)
{
	StaticData = InDataAsset;

	if (StaticData)
	{
		CurrentHealth = StaticData->BaseMaxHealth;
	}
}

void UStatComponent::TakeDamage(float DamageAmount)
{
	if (LinkedRuntimeData)
	{
		LinkedRuntimeData->ApplyDamage(DamageAmount);
	}
	else
	{
		CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);
		OnTakeDamage.Broadcast(CurrentHealth, GetMaxHealth());
	}
}

int32 UStatComponent::GetLevel() const
{
	return (StaticData != nullptr) ? 1 : 1;
}

float UStatComponent::GetCurrentHealth() const
{
	if (LinkedRuntimeData) return LinkedRuntimeData ? LinkedRuntimeData->GetCurrentHealth() : 0.0f;
	else return CurrentHealth;
}

float UStatComponent::GetMaxHealth() const
{
	return StaticData ? StaticData->BaseMaxHealth : 0.0f;
}

float UStatComponent::GetAttackPower() const
{
	return StaticData ? StaticData->BaseAttackPower : 0.0f;
}

float UStatComponent::GetDefense() const
{
	return StaticData ? StaticData->BaseDefensePower : 0.0f;
}

float UStatComponent::GetSpeed() const
{
	return StaticData ? StaticData->BaseSpeed : 0.0f;
}
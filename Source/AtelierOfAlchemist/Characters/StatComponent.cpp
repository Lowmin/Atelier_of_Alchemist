#include "StatComponent.h"
#include "../PlayerRuntimeData.h"     
#include "../DataAssets/CharacterDataAsset.h" 

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	LinkedRuntimeData = nullptr;
	StaticData = nullptr;
}

void UStatComponent::Initialize(UPlayerRuntimeData* InRuntimeData)
{
	LinkedRuntimeData = InRuntimeData;
	if (LinkedRuntimeData)
	{
		StaticData = LinkedRuntimeData->GetCharacterDataAsset();
	}
}

void UStatComponent::TakeDamage(float DamageAmount)
{
	if (LinkedRuntimeData)
	{
		LinkedRuntimeData->ApplyDamage(DamageAmount);
	}
}

int32 UStatComponent::GetLevel() const
{
	return (StaticData != nullptr) ? 1 : 1;
}

float UStatComponent::GetCurrentHealth() const
{
	return LinkedRuntimeData ? LinkedRuntimeData->GetCurrentHealth() : 0.0f;
}

float UStatComponent::GetMaxHealth() const
{
	return LinkedRuntimeData ? LinkedRuntimeData->GetMaxHealth() : 0.0f;
}

float UStatComponent::GetAttackPower() const
{
	return StaticData ? StaticData->BaseAttackPower : 0.0f;
}

float UStatComponent::GetDefense() const
{
	return StaticData ? StaticData->BaseDefensePower : 0.0f;
}
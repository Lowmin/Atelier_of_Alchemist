#include "StatComponent.h"
#include "../DataAssets/CharacterDataAsset.h"
#include "../DataAssets/EnemyDataAsset.h" 

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentHealth = 0.0f;
}

void UStatComponent::Initialize(UPlayerRuntimeData* InRuntimeData)
{
	LinkedRuntimeData = InRuntimeData;

	if (LinkedRuntimeData)
	{
		StaticData = LinkedRuntimeData->GetCharacterDataAsset();
		LinkedRuntimeData->OnHealthChanged.RemoveDynamic(this, &UStatComponent::OnRuntimeDataChanged);
		LinkedRuntimeData->OnHealthChanged.AddDynamic(this, &UStatComponent::OnRuntimeDataChanged);
	}
}

void UStatComponent::InitializeFromEnemy(UEnemyDataAsset* InDataAsset)
{
	StaticData = Cast<UCharacterDataAsset>(InDataAsset);

	if (StaticData)
	{
		CurrentHealth = GetMaxHealth();
		if (OnHealthChanged.IsBound())
		{
			OnHealthChanged.Broadcast(CurrentHealth, GetMaxHealth());
		}
	}
}

float UStatComponent::ApplyDamage(float InDamage)
{
	float PrevHP = GetCurrentHealth();
	float MaxHP = GetMaxHealth();
	float NewHP = FMath::Clamp(PrevHP - InDamage, 0.0f, MaxHP);
	float ActualDamage = PrevHP - NewHP;

	if (LinkedRuntimeData)
	{
		LinkedRuntimeData->SetCurrentHealth(NewHP);
	}
	else
	{
		CurrentHealth = NewHP;
		if (OnHealthChanged.IsBound())
		{
			OnHealthChanged.Broadcast(CurrentHealth, MaxHP);
		}
	}

	return ActualDamage;
}

void UStatComponent::Heal(float HealAmount)
{
	float PrevHP = GetCurrentHealth();
	float MaxHP = GetMaxHealth();
	float NewHP = FMath::Clamp(PrevHP + HealAmount, 0.0f, MaxHP);

	if (LinkedRuntimeData)
	{
		LinkedRuntimeData->SetCurrentHealth(NewHP);
	}
	else
	{
		CurrentHealth = NewHP;
		if (OnHealthChanged.IsBound())
		{
			OnHealthChanged.Broadcast(CurrentHealth, MaxHP);
		}
	}
}

float UStatComponent::GetCurrentHealth() const
{
	if (LinkedRuntimeData) return LinkedRuntimeData->GetCurrentHealth();
	return CurrentHealth;
}

float UStatComponent::GetMaxHealth() const
{
	if (LinkedRuntimeData) return LinkedRuntimeData->GetTotalMaxHealth();
	return StaticData ? StaticData->MaxHealth : 0.0f;
}

float UStatComponent::GetAttackPower() const
{
	if (LinkedRuntimeData) return LinkedRuntimeData->GetTotalAttack();
	return StaticData ? StaticData->AttackPower : 0.0f;
}

float UStatComponent::GetDefense() const
{
	if (LinkedRuntimeData) return LinkedRuntimeData->GetTotalDefense();
	return StaticData ? StaticData->DefensePower : 0.0f;
}

float UStatComponent::GetSpeed() const
{
	if (LinkedRuntimeData) return LinkedRuntimeData->GetTotalSpeed();
	return StaticData ? StaticData->Speed : 0.0f;
}

float UStatComponent::GetLevel() const
{
	if (LinkedRuntimeData) return LinkedRuntimeData->GetLevel();
	return 1.0f;
}

UCharacterDataAsset* UStatComponent::GetCharacterData() const
{
	return StaticData;
}

void UStatComponent::OnRuntimeDataChanged(float NewCurrent, float NewMax)
{
	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(NewCurrent, NewMax);
	}
}
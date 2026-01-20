#include "StatComponent.h"
#include "../PlayerRuntimeData.h"
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
	StaticData = InDataAsset;

	if (StaticData)
	{
		CurrentHealth = GetMaxHealth();

		if (OnHealthChanged.IsBound())
		{
			OnHealthChanged.Broadcast(CurrentHealth, GetMaxHealth());
		}
	}
}

void UStatComponent::TakeDamage(float Amount)
{
	if (LinkedRuntimeData)
	{
		LinkedRuntimeData->ApplyDamage(Amount);
		CurrentHealth = LinkedRuntimeData->GetCurrentHealth();
	}
	else
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0.0f, GetMaxHealth());
		if (OnHealthChanged.IsBound())
		{
			OnHealthChanged.Broadcast(CurrentHealth, GetMaxHealth());
		}
	}
}

void UStatComponent::Heal(float HealAmount)
{
	if (LinkedRuntimeData)
	{
		float CurHp = LinkedRuntimeData->GetCurrentHealth();
		float MaxHp = GetMaxHealth();

		float AfterHp = FMath::Min(CurHp + HealAmount, MaxHp);
		LinkedRuntimeData->SetCurrentHealth(AfterHp);
	}
	else
	{
		float MaxHp = GetMaxHealth();
		CurrentHealth = FMath::Min(CurrentHealth + HealAmount, MaxHp);

		if (OnHealthChanged.IsBound())
		{
			OnHealthChanged.Broadcast(CurrentHealth, MaxHp);
		}
	}
}

float UStatComponent::GetCurrentHealth() const
{
	if (LinkedRuntimeData)
	{
		return LinkedRuntimeData->GetCurrentHealth();
	}

	return CurrentHealth;
}

float UStatComponent::GetMaxHealth() const
{
	if (LinkedRuntimeData) return LinkedRuntimeData->GetTotalMaxHealth();
	return StaticData ? StaticData->BaseMaxHealth : 0.0f;
}

float UStatComponent::GetAttackPower() const
{
	if (LinkedRuntimeData) return LinkedRuntimeData->GetTotalAttack();
	return StaticData ? StaticData->BaseAttackPower : 0.0f;
}

float UStatComponent::GetDefense() const
{
	if (LinkedRuntimeData) return LinkedRuntimeData->GetTotalDefense();
	return StaticData ? StaticData->BaseDefensePower : 0.0f;
}

float UStatComponent::GetSpeed() const
{
	if (LinkedRuntimeData) return LinkedRuntimeData->GetTotalSpeed();
	return StaticData ? StaticData->BaseSpeed : 0.0f;
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
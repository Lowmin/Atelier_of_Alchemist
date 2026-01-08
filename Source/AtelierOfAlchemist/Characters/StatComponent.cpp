#include "StatComponent.h"
#include "../PlayerRuntimeData.h"     
#include "../DataAssets/CharacterDataAsset.h" 
#include "../DataAssets/ItemDataAsset.h"

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

		LinkedRuntimeData->OnHealthChanged.RemoveDynamic(this, &UStatComponent::OnRuntimeDataChanged);
		LinkedRuntimeData->OnHealthChanged.AddDynamic(this, &UStatComponent::OnRuntimeDataChanged);
	}
}

void UStatComponent::InitializeFromEnemy(UCharacterDataAsset* InDataAsset)
{
	StaticData = InDataAsset;

	if (StaticData)
	{
		CurrentHealth = StaticData->BaseMaxHealth;
		OnHealthChanged.Broadcast(CurrentHealth, StaticData->BaseMaxHealth);
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
		CurrentHealth -= Amount;
		if (OnHealthChanged.IsBound()) OnHealthChanged.Broadcast(CurrentHealth, GetMaxHealth());
	}
}

void UStatComponent::Heal(float HealAmount)
{
	if (LinkedRuntimeData && StaticData)
	{
		float CurHp = LinkedRuntimeData->GetCurrentHealth();
		float MaxHp = GetMaxHealth();

		float AfterHp = FMath::Min(CurHp + HealAmount, MaxHp);
		LinkedRuntimeData->SetCurrentHealth(AfterHp);
	}
	else if (StaticData)
	{
		float MaxHp = GetMaxHealth();
		CurrentHealth = FMath::Min(CurrentHealth + HealAmount, MaxHp);

		OnHealthChanged.Broadcast(CurrentHealth, MaxHp);
	}
}

void UStatComponent::AddEquipStat(UItemDataAsset* EquipData)
{
	BonusAttackPower += EquipData->EquipAttackPower;
	BonusDefense += EquipData->EquipDefense;
	BonusHealth += EquipData->EquipMaxHealth;
}

void UStatComponent::RemoveEquipStat(UItemDataAsset* EquipData)
{
	BonusAttackPower -= EquipData->EquipAttackPower;
	BonusDefense -= EquipData->EquipDefense;
	BonusHealth -= EquipData->EquipMaxHealth;

	BonusAttackPower = FMath::Max(BonusAttackPower, 0.0f);
	BonusDefense = FMath::Max(BonusDefense, 0.0f);
	BonusHealth = FMath::Max(BonusHealth, 0.0f);
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
	return StaticData ? StaticData->BaseMaxHealth + BonusHealth : 0.0f;
}

float UStatComponent::GetAttackPower() const
{
	return StaticData ? StaticData->BaseAttackPower + BonusAttackPower : 0.0f;
}

float UStatComponent::GetDefense() const
{
	return StaticData ? StaticData->BaseDefensePower + BonusDefense : 0.0f;
}

float UStatComponent::GetSpeed() const
{
	return StaticData ? StaticData->BaseSpeed + BonusSpeed : 0.0f;
}

UCharacterDataAsset* UStatComponent::GetCharacterData() const
{
	return StaticData;
}

void UStatComponent::OnRuntimeDataChanged(float NewCurrent, float NewMax)
{
	OnHealthChanged.Broadcast(NewCurrent, NewMax);
}

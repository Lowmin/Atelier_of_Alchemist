#include "PlayerRuntimeData.h"
#include "DataAssets/CharacterDataAsset.h"

void UPlayerRuntimeData::Initialize(UCharacterDataAsset* NewCharacterDataAsset)
{
	this->CharacterDataAsset = NewCharacterDataAsset;

	if (CharacterDataAsset != nullptr)
	{
		MaxHealth = CharacterDataAsset->BaseMaxHealth;
		CurrentHealth = MaxHealth;
		CharacterID = CharacterDataAsset->GetFName();
		Level = 1;
	}
}

void UPlayerRuntimeData::ApplyDamage(float dmg)
{
	CurrentHealth -= dmg;
	OnHealthChanged.Broadcast(CurrentHealth, GetTotalMaxHealth());
}

void UPlayerRuntimeData::SetCurrentHealth(float NewHp)
{
	if (CurrentHealth != NewHp)
		CurrentHealth = NewHp;
	OnHealthChanged.Broadcast(CurrentHealth, GetTotalMaxHealth());
}

UItemDataAsset* UPlayerRuntimeData::GetEquipItem(EEquipPart Part) const
{
	if (EquippedItems.Contains(Part))
	{
		return EquippedItems[Part];
	}
	return nullptr;
}

void UPlayerRuntimeData::SetEquipItem(EEquipPart Part, UItemDataAsset* Item)
{
	if (Item)
	{
		EquippedItems.Add(Part, Item);
	}
	else
	{
		EquippedItems.Remove(Part);
	}

	if (OnEquipChanged.IsBound())
	{
		OnEquipChanged.Broadcast();
	}

	OnHealthChanged.Broadcast(CurrentHealth, GetTotalMaxHealth());
}

float UPlayerRuntimeData::GetTotalAttack() const
{
	if (!CharacterDataAsset) return 0.0f;

	float Total = CharacterDataAsset->BaseAttackPower;

	for (const auto& Pair : EquippedItems)
	{
		if (Pair.Value) Total += Pair.Value->EquipAttackPower;
	}
	return Total;
}

float UPlayerRuntimeData::GetTotalDefense() const
{
	if (!CharacterDataAsset) return 0.0f;

	float Total = CharacterDataAsset->BaseDefensePower;

	for (const auto& Pair : EquippedItems)
	{
		if (Pair.Value) Total += Pair.Value->EquipDefense;
	}
	return Total;
}

float UPlayerRuntimeData::GetTotalMaxHealth() const
{
	if (!CharacterDataAsset) return 0.0f;

	float Total = CharacterDataAsset->BaseMaxHealth;

	for (const auto& Pair : EquippedItems)
	{
		if (Pair.Value) Total += Pair.Value->EquipMaxHealth;
	}
	return Total;
}

float UPlayerRuntimeData::GetTotalSpeed() const
{
	if (!CharacterDataAsset) return 0.0f;
	return CharacterDataAsset->BaseSpeed;
}
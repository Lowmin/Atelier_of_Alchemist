#include "PlayerRuntimeData.h"
#include "DataAssets/CharacterDataAsset.h"
#include "DataAssets/ItemDataAsset.h"
#include "DataAssets/GradeHelper.h"

void UPlayerRuntimeData::Initialize(UCharacterDataAsset* NewCharacterDataAsset)
{
	if (!NewCharacterDataAsset) return;

	CharacterDataAsset = NewCharacterDataAsset;

	if (CharacterDataAsset->CharacterID.IsNone())
	{
		CharacterID = CharacterDataAsset->GetFName();
	}
	else
	{
		CharacterID = CharacterDataAsset->CharacterID;
	}

	MaxHealth = CharacterDataAsset->MaxHealth;
	CurrentHealth = MaxHealth;
	Level = 1;

	EquippedItems.Empty();
	EquippedGrades.Empty();
}

void UPlayerRuntimeData::SetCurrentHealth(float NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0.0f, GetTotalMaxHealth());
	OnHealthChanged.Broadcast(CurrentHealth, GetTotalMaxHealth());
}

void UPlayerRuntimeData::SetEquipItem(EEquipPart Part, UItemDataAsset* Item, EItemGrade Grade)
{
	if (Item)
	{
		EquippedItems.Add(Part, Item);
		EquippedGrades.Add(Part, Grade);
	}
	else
	{
		EquippedItems.Remove(Part);
		EquippedGrades.Remove(Part);
	}

	OnEquipChanged.Broadcast();

	float TotalMaxHP = GetTotalMaxHealth();
	if (CurrentHealth > TotalMaxHP)
	{
		CurrentHealth = TotalMaxHP;
	}
	OnHealthChanged.Broadcast(CurrentHealth, TotalMaxHP);
}

UItemDataAsset* UPlayerRuntimeData::GetEquipItem(EEquipPart Part) const
{
	if (EquippedItems.Contains(Part))
	{
		return EquippedItems[Part];
	}
	return nullptr;
}

EItemGrade UPlayerRuntimeData::GetEquipGrade(EEquipPart Part) const
{
	if (EquippedGrades.Contains(Part))
	{
		return EquippedGrades[Part];
	}
	return EItemGrade::EIG_E;
}

float UPlayerRuntimeData::GetTotalMaxHealth() const
{
	if (!CharacterDataAsset) return 0.0f;

	float Total = CharacterDataAsset->MaxHealth;

	for (const auto& Pair : EquippedItems)
	{
		UItemDataAsset* Item = Pair.Value;
		if (Item)
		{
			EItemGrade Grade = GetEquipGrade(Pair.Key);
			float Multiplier = AlchemyMath::GetGradeMultiplier(Grade);
			Total += Item->EquipMaxHealth * Multiplier;
		}
	}
	return Total;
}

float UPlayerRuntimeData::GetTotalAttack() const
{
	if (!CharacterDataAsset) return 0.0f;

	float Total = CharacterDataAsset->AttackPower;

	for (const auto& Pair : EquippedItems)
	{
		UItemDataAsset* Item = Pair.Value;
		if (Item)
		{
			EItemGrade Grade = GetEquipGrade(Pair.Key);
			float Multiplier = AlchemyMath::GetGradeMultiplier(Grade);
			Total += Item->EquipAttackPower * Multiplier;
		}
	}
	return Total;
}

float UPlayerRuntimeData::GetTotalDefense() const
{
	if (!CharacterDataAsset) return 0.0f;

	float Total = CharacterDataAsset->DefensePower;

	for (const auto& Pair : EquippedItems)
	{
		UItemDataAsset* Item = Pair.Value;
		if (Item)
		{
			EItemGrade Grade = GetEquipGrade(Pair.Key);
			float Multiplier = AlchemyMath::GetGradeMultiplier(Grade);
			Total += Item->EquipDefense * Multiplier;
		}
	}
	return Total;
}

float UPlayerRuntimeData::GetTotalSpeed() const
{
	if (!CharacterDataAsset) return 0.0f;

	float Total = CharacterDataAsset->Speed;

	for (const auto& Pair : EquippedItems)
	{
		UItemDataAsset* Item = Pair.Value;
		if (Item)
		{
			EItemGrade Grade = GetEquipGrade(Pair.Key);
			float Multiplier = AlchemyMath::GetGradeMultiplier(Grade);
			Total += Item->EquipSpeed * Multiplier;
		}
	}
	return Total;
}
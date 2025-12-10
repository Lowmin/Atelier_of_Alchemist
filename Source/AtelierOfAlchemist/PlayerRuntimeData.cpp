// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRuntimeData.h"
#include "DataAssets/CharacterDataAsset.h"

void UPlayerRuntimeData::Initialize(UCharacterDataAsset* NewCharacterDataAsset)
{
	this->CharacterDataAsset = NewCharacterDataAsset;

	if (CharacterDataAsset != nullptr)
	{
		MaxHealth = CharacterDataAsset->BaseMaxHealth;
		CurrentHealth = MaxHealth;
	}
}

void UPlayerRuntimeData::ApplyDamage(float dmg)
{
	CurrentHealth -= dmg;

	OnHealthChanged.Broadcast(CurrentHealth, GetMaxHealth());
}

void UPlayerRuntimeData::SetCurrentHealth(float NewHp)
{
	if (CurrentHealth != NewHp)
		CurrentHealth = NewHp;
	OnHealthChanged.Broadcast(CurrentHealth, GetMaxHealth());
}

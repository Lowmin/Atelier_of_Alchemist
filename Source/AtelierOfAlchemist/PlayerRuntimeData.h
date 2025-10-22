// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerRuntimeData.generated.h"

class UCharacterDataAsset;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);

UCLASS()
class ATELIEROFALCHEMIST_API UPlayerRuntimeData : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(UCharacterDataAsset* NewCharacterDataAsset);
	void ApplyDamage(float dmg);

	UCharacterDataAsset* GetCharacterDataAsset() const { return CharacterDataAsset; };
	float GetCurrentHealth() const { return CurrentHealth; };
	float GetMaxHealth() const { return MaxHealth; };

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

protected:
	UPROPERTY()
	TObjectPtr<UCharacterDataAsset> CharacterDataAsset;

	UPROPERTY()
	float CurrentHealth;

	UPROPERTY()
	float MaxHealth;
};

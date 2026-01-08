#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../PlayerRuntimeData.h"
#include "StatComponent.generated.h"

class UCharacterDataAsset;
class UItemDataAsset;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ATELIEROFALCHEMIST_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

	void Initialize(UPlayerRuntimeData* InRuntimeData);
	void InitializeFromEnemy(UCharacterDataAsset* InDataAsset);

	void TakeDamage(float DamageAmount);
	void Heal(float HealAmount);

	void AddEquipStat(UItemDataAsset* EquipData);
	void RemoveEquipStat(UItemDataAsset* EquipData);

	int32 GetLevel() const;
	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	float GetAttackPower() const;
	float GetDefense() const;
	float GetSpeed() const;

	UCharacterDataAsset* GetCharacterData() const;
	UPlayerRuntimeData* GetRuntimeData() const { return LinkedRuntimeData; }

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

protected:
	float BonusAttackPower;
	float BonusDefense;
	float BonusHealth;
	float BonusSpeed;

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeData> LinkedRuntimeData;

	UPROPERTY()
	TObjectPtr<UCharacterDataAsset> StaticData;

	float CurrentHealth;

	UFUNCTION()
	void OnRuntimeDataChanged(float NewCurrent, float NewMax);
};
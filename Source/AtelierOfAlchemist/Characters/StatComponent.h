#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../PlayerRuntimeData.h" 
#include "StatComponent.generated.h"

class UEnemyDataAsset;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ATELIEROFALCHEMIST_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

	void Initialize(UPlayerRuntimeData* InRuntimeData);
	void InitializeFromEnemy(UEnemyDataAsset* InDataAsset);

	float ApplyDamage(float InDamage);
	void Heal(float HealAmount);

	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	float GetAttackPower() const;
	float GetDefense() const;
	float GetSpeed() const;
	float GetLevel() const;

	UCharacterDataAsset* GetCharacterData() const;
	UPlayerRuntimeData* GetRuntimeData() const { return LinkedRuntimeData; }

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

protected:
	UPROPERTY()
	TObjectPtr<UPlayerRuntimeData> LinkedRuntimeData;

	UPROPERTY()
	TObjectPtr<UCharacterDataAsset> StaticData;

	float CurrentHealth;

	UFUNCTION()
	void OnRuntimeDataChanged(float NewCurrent, float NewMax);
};
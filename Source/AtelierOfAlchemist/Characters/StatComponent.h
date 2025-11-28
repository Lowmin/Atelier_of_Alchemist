#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

class UPlayerRuntimeData;
class UCharacterDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTakeDamage, float, CurrnetHealth, float, MaxHealth);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ATELIEROFALCHEMIST_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

	void Initialize(UPlayerRuntimeData* InRuntimeData);
	void InitializeFromEnemy(UCharacterDataAsset* InDataAsset);

	void TakeDamage(float DamageAmount);

	int32 GetLevel() const;
	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	float GetAttackPower() const;
	float GetDefense() const;
	float GetSpeed() const;

	UPROPERTY(BlueprintAssignable)
	FOnTakeDamage OnTakeDamage;

protected:
	UPROPERTY()
	TObjectPtr<UPlayerRuntimeData> LinkedRuntimeData;

	UPROPERTY()
	TObjectPtr<UCharacterDataAsset> StaticData;

	float CurrentHealth;
};
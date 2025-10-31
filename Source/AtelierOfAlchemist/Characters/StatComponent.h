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

	UPROPERTY(BlueprintAssignable)
	FOnTakeDamage OnTakeDamage;

protected:
	/** * 이 컴포넌트가 참조하는 '진짜' 데이터 원본 (GuildMemberManager가 소유).
	 * UPROPERTY()로 GC(가비지 컬렉션)를 방지해야 합니다.
	 */
	UPROPERTY()
	TObjectPtr<UPlayerRuntimeData> LinkedRuntimeData;

	/** 편의를 위한 DataAsset 캐시 */
	UPROPERTY()
	TObjectPtr<UCharacterDataAsset> StaticData;

	float CurrentHealth;
};
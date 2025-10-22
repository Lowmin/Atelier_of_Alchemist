#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

class UPlayerRuntimeData; // 데이터 원본 UObject
class UCharacterDataAsset; // 고정 데이터

/**
 * 액터(ACharacterBase)에 부착되어 실시간 데이터(UPlayerRuntimeData)에 대한
 * 인터페이스 역할을 하는 컴포넌트입니다.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ATELIEROFALCHEMIST_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

	/** APlayerCharacter의 BeginPlay에서 호출되어 데이터 원본과 연결합니다. */
	void Initialize(UPlayerRuntimeData* InRuntimeData);

	/** ACharacterBase의 TakeDamage로부터 호출됩니다. */
	void TakeDamage(float DamageAmount);

	/** ACharacterBase의 Getter들이 호출할 함수들입니다. */
	int32 GetLevel() const;
	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	float GetAttackPower() const;
	float GetDefense() const;

protected:
	/** * 이 컴포넌트가 참조하는 '진짜' 데이터 원본 (GuildMemberManager가 소유).
	 * UPROPERTY()로 GC(가비지 컬렉션)를 방지해야 합니다.
	 */
	UPROPERTY()
	TObjectPtr<UPlayerRuntimeData> LinkedRuntimeData;

	/** 편의를 위한 DataAsset 캐시 */
	UPROPERTY()
	TObjectPtr<UCharacterDataAsset> StaticData;
};
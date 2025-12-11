// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StatComponent.h"
#include "CharacterBase.generated.h"

class USkillDataAsset;
class UCharacterDataAsset;

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Player	UMETA(DisplayName = "Player"),
	Enemy	UMETA(DisplayName = "Enemy"),
	NPC		UMETA(DisplayName = "NPC")
};

UCLASS()
class ATELIEROFALCHEMIST_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle")
	ECharacterType Type;

	virtual void BattleAction_UseSkill(USkillDataAsset* Skill, const TArray<ACharacterBase*>& Targets);

	virtual void OnTurnStart();

	int32 GetLevel() const;
	float GetCurHealth() const;
	float GetMaxHealth() const;
	float GetAttackPower() const;
	float GetDefense() const;
	float GetSpeed() const;

	FORCEINLINE UStatComponent* GetStatComponent() const { return StatComponent; }
	UCharacterDataAsset* GetCharacterData() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle")
	TArray<USkillDataAsset*> SkillList;

	// [추가] 스킬 가져오는 함수
	USkillDataAsset* GetSkill(int32 Index) const;

	bool IsPlayerTeam() const { return Type == ECharacterType::Player; }

protected:
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStatComponent> StatComponent;
};
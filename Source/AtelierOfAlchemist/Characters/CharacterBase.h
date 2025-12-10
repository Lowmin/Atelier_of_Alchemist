// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <AtelierOfAlchemist/DataAssets/SkillDataAsset.h>
#include "StatComponent.h"
#include "CharacterBase.generated.h"

class USkillListComponent;
class USkillDataAsset;
class UCharacterDataAsset;

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Player  UMETA(DisplayName = "Player"),
	Enemy	UMETA(DisplayName = "Enemy"),
	NPC		UMETA(DisplayName = "NPC")
};

UCLASS()
class ATELIEROFALCHEMIST_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStatComponent> StatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkillListComponent> SkillComponent;

public:
	ACharacterBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle")
	ECharacterType Type;

protected:
	virtual void BeginPlay() override;
	virtual void BattleAction_UseSkill(USkillDataAsset* Skill, const TArray<ACharacterBase*>& Targets);

public:
	virtual void Tick(float DeltaTime) override;

	int32 GetLevel();
	float GetCurHealth();
	float GetMaxHealth();
	float GetAttackPower();
	float GetDefense();
	float GetSpeed();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE UStatComponent* GetStatComponent() const { return StatComponent; }
	UCharacterDataAsset* GetCharacterData() const { return StatComponent->GetCharacterData(); };
};

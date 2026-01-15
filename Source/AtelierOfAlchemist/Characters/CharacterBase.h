// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StatComponent.h"
#include "../DataAssets/ItemDataAsset.h"
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

	// 데이터
	FORCEINLINE UStatComponent* GetStatComponent() const { return StatComponent; }
	UCharacterDataAsset* GetCharacterData() const;
	int32 GetLevel() const;
	float GetCurHealth() const;
	float GetMaxHealth() const;
	float GetAttackPower() const;
	float GetDefense() const;
	float GetSpeed() const;

	bool IsPlayerTeam() const { return Type == ECharacterType::Player; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStatComponent> StatComponent;
};

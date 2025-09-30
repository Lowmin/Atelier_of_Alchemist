// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ATELIEROFALCHEMIST_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

private:

	// ½ºÅÈ °ü·Ã
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	int32 Level{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float CurHealth{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float MaxHealth{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float AttackPower{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float Defense{};

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void TakeDamage();
	virtual void Attack();
	virtual void Die();

public:	
	virtual void Tick(float DeltaTime) override;
	
	int32 GetLevel();
	float GetCurHealth();
	float GetMaxHealth();
	float GetAttackPower();
	float GetDefense();

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

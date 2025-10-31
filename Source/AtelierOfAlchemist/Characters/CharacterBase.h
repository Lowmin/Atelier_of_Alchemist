// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UStatComponent;

UCLASS()
class ATELIEROFALCHEMIST_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStatComponent> StatComponent;

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void Attack();
	virtual void Die();

public:
	virtual void Tick(float DeltaTime) override;

	int32 GetLevel();
	float GetCurHealth();
	float GetMaxHealth();
	float GetAttackPower();
	float GetDefense();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE UStatComponent* GetStatComponent() const { return StatComponent; }
};

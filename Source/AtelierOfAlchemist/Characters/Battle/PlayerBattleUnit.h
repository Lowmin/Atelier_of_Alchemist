// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleUnit.h"
#include "PlayerBattleUnit.generated.h"

/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API APlayerBattleUnit : public ABattleUnit
{
	GENERATED_BODY()
	
public:
	APlayerBattleUnit();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void Dodge(); 

	UFUNCTION(BlueprintCallable)
	void OnAnimNotify_Dodge();

protected:
	void DodgeEnd();

	bool bIsDodge = false;

	FTimerHandle TimerHandle_Dodge;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DodgeDuration = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> DodgeMontage;
};

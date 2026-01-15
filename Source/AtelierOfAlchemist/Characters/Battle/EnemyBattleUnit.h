// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleUnit.h"
#include "EnemyBattleUnit.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API AEnemyBattleUnit : public ABattleUnit
{
	GENERATED_BODY()
	
public:
	AEnemyBattleUnit();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void TurnStart() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> HealthBarWidget;

	UFUNCTION()
	void UpdateStatusHp(float CurrentHp, float MaxHp);
};

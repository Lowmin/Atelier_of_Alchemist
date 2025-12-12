// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "Enemy.generated.h"

class UWidgetComponent;
class UCharacterDataAsset;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API AEnemy : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemy();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> StatusWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Data")
	TObjectPtr<UCharacterDataAsset> EnemyData;

	UFUNCTION()
	void UpdateStatusHp(float CurrentHp, float MaxHp);
};

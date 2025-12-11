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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> StatusWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Data")
	TObjectPtr<UCharacterDataAsset> EnemyData;

	UFUNCTION()
	void UpdateStatusWidget(float CurrentHp, float MaxHp);
};

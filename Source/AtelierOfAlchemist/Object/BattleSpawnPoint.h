// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleSpawnPoint.generated.h"

class UArrowComponent;

UCLASS()
class ATELIEROFALCHEMIST_API ABattleSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ABattleSpawnPoint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 SpawnIndex{};
};

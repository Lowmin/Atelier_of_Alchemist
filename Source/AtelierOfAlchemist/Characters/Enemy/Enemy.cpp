// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "../StatComponent.h"
#include "../../DataAssets/CharacterDataAsset.h"
#include "AIController.h"

AEnemy::AEnemy()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (StatComponent && EnemyData)
	{
		StatComponent->InitializeFromEnemy(EnemyData);
	}

	if (AAIController* AIController = Cast <AAIController>(GetController()))
	{
		// AIController->RunBehaviorTree();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "../StatComponent.h"
#include "../../DataAssets/CharacterDataAsset.h"
#include "Components/WidgetComponent.h"
#include "AIController.h"

AEnemy::AEnemy()
{
	StatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusWidgetComponent"));
	StatusWidgetComponent->SetupAttachment(GetRootComponent());

	StatusWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	
	StatusWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	Type = ECharacterType::Enemy;
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

void AEnemy::UpdateStatusWidget(float CurrentHp, float MaxHp)
{

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "../StatComponent.h"
#include "../../DataAssets/CharacterDataAsset.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../UI/Battle/EnemyStatusWidget.h"
#include "AIController.h"

AEnemy::AEnemy()
{
	StatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusWidgetComponent"));
	StatusWidgetComponent->SetupAttachment(GetRootComponent());
	StatusWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	Type = ECharacterType::Enemy;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		float Height = Capsule->GetScaledCapsuleHalfHeight();
		float WidgetHeight = Height + 20.0f;
		StatusWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, WidgetHeight));
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (StatComponent && EnemyData)
	{
		StatComponent->InitializeFromEnemy(EnemyData);

		if (UEnemyStatusWidget* EnemyStatusWidget = Cast<UEnemyStatusWidget>(StatusWidgetComponent->GetUserWidgetObject()))
		{
			UpdateStatusHp(StatComponent->GetCurrentHealth(), StatComponent->GetMaxHealth());
			EnemyStatusWidget->SetLevel(StatComponent->GetLevel());
			EnemyStatusWidget->SetName(EnemyData->CharacterName);
		}
	}

	if (AAIController* AIController = Cast <AAIController>(GetController()))
	{
		// AIController->RunBehaviorTree();
	}
}

void AEnemy::UpdateStatusHp(float CurrentHp, float MaxHp)
{
	if (UEnemyStatusWidget* EnemyStatusWidget = Cast<UEnemyStatusWidget>(StatusWidgetComponent->GetUserWidgetObject()))
	{
		EnemyStatusWidget->UpdateHp(CurrentHp, MaxHp);
	}
}

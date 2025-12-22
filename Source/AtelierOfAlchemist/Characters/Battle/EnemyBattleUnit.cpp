// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBattleUnit.h"
#include "../../BattleGameMode.h"
#include "Components/WidgetComponent.h"
#include "../../UI/Battle/EnemyStatusWidget.h"
#include "../Enemy/EnemyAIController.h"

AEnemyBattleUnit::AEnemyBattleUnit()
{
	Type = ECharacterType::Enemy;

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(150.0f, 20.0f));
	HealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
}

void AEnemyBattleUnit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

void AEnemyBattleUnit::BeginPlay()
{
	Super::BeginPlay();

	if (StatComponent)
	{
		StatComponent->OnHealthChanged.AddDynamic(this, &AEnemyBattleUnit::UpdateStatusHp);
	}

	if (HealthBarWidget)
	{
		UEnemyStatusWidget* EnemyWidget = Cast<UEnemyStatusWidget>(HealthBarWidget->GetUserWidgetObject());

		if (EnemyWidget && StatComponent)
		{
			EnemyWidget->BindStatComponent(StatComponent);
		}
	}
}

void AEnemyBattleUnit::TurnStart()
{
	Super::TurnStart();

	if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController()))
	{
		EnemyAIController->SetTurnStatus(true);
	}
}

void AEnemyBattleUnit::UpdateStatusHp(float CurrentHp, float MaxHp)
{
	if (HealthBarWidget)
	{
		if (UEnemyStatusWidget* HPBar = Cast<UEnemyStatusWidget>(HealthBarWidget->GetUserWidgetObject()))
		{
			HPBar->UpdateHp(CurrentHp, MaxHp);
		}
	}
}
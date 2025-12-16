// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBattleUnit.h"
#include "../../BattleGameMode.h"
#include "Components/WidgetComponent.h"
#include "../../UI/Battle/EnemyStatusWidget.h"

AEnemyBattleUnit::AEnemyBattleUnit()
{
	Type = ECharacterType::Enemy;
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
}

void AEnemyBattleUnit::TurnStart()
{
	Super::TurnStart();

	if (ABattleGameMode* BattleGameMode = GetWorld()->GetAuthGameMode<ABattleGameMode>())
	{
		BattleGameMode->ExecuteEnemyTurn();
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
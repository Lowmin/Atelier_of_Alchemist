#include "EnemyBattleUnit.h"
#include "../../BattleGameMode.h"
#include "Components/WidgetComponent.h"
#include "../Enemy/EnemyAIController.h"
#include "../../AoABattleController.h"

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

}

void AEnemyBattleUnit::UpdateStatusHp(float CurrentHp, float MaxHp)
{

}

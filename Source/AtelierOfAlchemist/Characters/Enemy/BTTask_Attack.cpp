// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "../Battle/BattleUnit.h"
#include "../Enemy/EnemyAIController.h"
#include "../../BattleGameMode.h"
#include "../../DataAssets/SkillDataAsset.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack Target");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	ABattleUnit* MyUnit = Cast<ABattleUnit>(AIController->GetPawn());

	ABattleGameMode* BattleGameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	const TArray<ABattleUnit*>& AllUnits = BattleGameMode->GetAllUnits();
	TArray<ABattleUnit*> ValidTargets;

	for (ABattleUnit* Unit : AllUnits)
	{
		if (Unit->Type == ECharacterType::Player)
		{
			ValidTargets.Add(Unit);
		}
	}

	int32 RandomIndex = FMath::RandRange(0, ValidTargets.Num() - 1);
	ABattleUnit* SelectedTarget = ValidTargets[RandomIndex];

	USkillDataAsset* Skill = MyUnit->GetSkill(0);
	if (!Skill)
	{
		return EBTNodeResult::Failed;
	}

	TArray<ABattleUnit*> FinalTargets;
	FinalTargets.Add(SelectedTarget);

	MyUnit->BattleAction_UseSkill(Skill, FinalTargets);

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(AIController);
	EnemyAIController->SetTurnStatus(false);

	return EBTNodeResult::Succeeded;
}

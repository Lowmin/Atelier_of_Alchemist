// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"

const FName AEnemyAIController::BBKey_IsMyTurn(TEXT("IsMyTurn"));
const FName AEnemyAIController::BBKey_TargetActor(TEXT("TargetActor"));

AEnemyAIController::AEnemyAIController()
{

}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	BlackboardComponent = nullptr;

	if (BTAsset && BBAsset)
	{
		if (UseBlackboard(BBAsset, BlackboardComponent))
		{
			Blackboard = BlackboardComponent;

			RunBehaviorTree(BTAsset);
			Blackboard->SetValueAsBool(BBKey_IsMyTurn, false);
		}
	}
}

void AEnemyAIController::SetTurnStatus(bool bIsMyTurn)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsBool(BBKey_IsMyTurn, bIsMyTurn);
	}
}

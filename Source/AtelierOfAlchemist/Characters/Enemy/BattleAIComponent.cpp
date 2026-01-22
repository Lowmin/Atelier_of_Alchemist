#include "BattleAIComponent.h"
#include "../../BattleGameMode.h"
#include "Kismet/GameplayStatics.h"

UBattleAIComponent::UBattleAIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBattleAIComponent::RunAI()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBattleAIComponent::DecideAction, 1.0f, false);
}

void UBattleAIComponent::DecideAction()
{
	ABattleGameMode* BattleGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());
	ABattleUnit* Owner = Cast<ABattleUnit>(GetOwner());

	for (int32 i = 0; i < AIPatterns.Num(); i++)
	{
		FAIPattern Pattern = AIPatterns[i];

		// 한 번만 실행하는 패턴 && 사용한 패턴 목록에 이 패턴이 포함되어 있는가? 
		if (AIPatterns[i].bIsOneTimeOnly == true && UsedPatternIndices.Contains(i)) continue;

		// AI 패턴에 해당하는 조건이 있는가?
		if (CheckCondition(AIPatterns[i]) == false) continue;

		ABattleUnit* FinalTarget = FindTarget(Pattern.TargetType);

		// 대상을 찾을 수 있는가?
		if (FinalTarget == nullptr) continue;

		UE_LOG(LogTemp, Warning, TEXT("[AI] Action: %s uses %s on %s"),
			*Owner->GetActorLabel(),
			*Pattern.SkillAsset->GetName(),
			*FinalTarget->GetActorLabel());

		BattleGameMode->ExecuteAction(Owner, FinalTarget, Pattern.SkillAsset);
		if (Pattern.bIsOneTimeOnly == true) UsedPatternIndices.Add(i);

		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[AI] No Action. Skipping Turn."));
	BattleGameMode->ProcessNextTurn();
}

bool UBattleAIComponent::CheckCondition(FAIPattern Pattern)
{
	ABattleUnit* OwnerUnit = Cast<ABattleUnit>(GetOwner());
	if (!OwnerUnit) return false;

	UStatComponent* MyStat = OwnerUnit->GetStatComponent();
	if (!MyStat) return false;
	
	ABattleGameMode* BattleGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());

	switch (Pattern.ConditionType)
	{
	case EAIConditionType::None:
		return true;
	case EAIConditionType::Chance:
		return (FMath::RandRange(0.0f, 100.0f) <= Pattern.ConditionValue);
	case EAIConditionType::SelfHP_Under:
	{
		float CurrentHPPercent = (MyStat->GetCurrentHealth() / MyStat->GetMaxHealth()) * 100.0f;;
		return (CurrentHPPercent <= Pattern.ConditionValue);
	}
	case EAIConditionType::AllyHP_Under:
	{
		for (ABattleUnit* Unit : BattleGameMode->GetAllUnits())
		{
			if (Unit && Unit->GetStatComponent()->GetCurrentHealth() > 0 && Unit->IsPlayerTeam() == OwnerUnit->IsPlayerTeam())
			{
				UStatComponent* AllyStat = Unit->GetStatComponent();
				float AllyHPPercent = (AllyStat->GetCurrentHealth() / AllyStat->GetMaxHealth()) * 100.0f;

				if (AllyHPPercent <= Pattern.ConditionValue) return true;
			}
		}
		return false;
	}
	case EAIConditionType::Turn_Multiple:
		return false;
	case EAIConditionType::Turn_Special:
		return false;
	}

	return false;
}

ABattleUnit* UBattleAIComponent::FindTarget(EAITargetType TargetType)
{
	ABattleGameMode* BattleGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());
	ABattleUnit* OwnerUnit = Cast<ABattleUnit>(GetOwner());
	if (!OwnerUnit) return nullptr;

	TArray<ABattleUnit*> Candidates;
	ABattleUnit* BestUnit = nullptr;
	float LowHPPercent = 200.0f;

	switch (TargetType)
	{
	case EAITargetType::Self:
		return Cast<ABattleUnit>(GetOwner());
	case EAITargetType::Player_Random:
	{
		for (ABattleUnit* Unit : BattleGameMode->GetAllUnits())
		{
			if (Unit && Unit->GetStatComponent()->GetCurrentHealth() > 0 && Unit->IsPlayerTeam() != OwnerUnit->IsPlayerTeam()) 
			{
				Candidates.Add(Unit);
			}
		}

		if (Candidates.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, Candidates.Num() - 1);
			BestUnit = Candidates[RandomIndex];
		}

		return BestUnit;
	}
	case EAITargetType::Player_Weakest:
	{
		for (ABattleUnit* Unit : BattleGameMode->GetAllUnits())
		{
			if (Unit && Unit->GetStatComponent()->GetCurrentHealth() > 0 && Unit->IsPlayerTeam() != OwnerUnit->IsPlayerTeam())
			{
				float UnitHPPercent = (Unit->GetStatComponent()->GetCurrentHealth() / Unit->GetStatComponent()->GetMaxHealth());
				if (LowHPPercent > UnitHPPercent)
				{
					LowHPPercent = UnitHPPercent;
					Candidates.Empty();
					Candidates.Add(Unit);
				}
				else if (LowHPPercent == UnitHPPercent)
				{
					Candidates.Add(Unit);
				}
			}
		}

		if (Candidates.Num() > 0)
		{
			float RandomIndex = FMath::RandRange(0, Candidates.Num() - 1);
			BestUnit = Candidates[RandomIndex];
		}

		return BestUnit;
	}
	case EAITargetType::Ally_Weakest:
	{
		for (ABattleUnit* Unit : BattleGameMode->GetAllUnits())
		{
			if (Unit && Unit->GetStatComponent()->GetCurrentHealth() > 0 && Unit->IsPlayerTeam() == OwnerUnit->IsPlayerTeam())
			{
				float UnitHPPercent = (Unit->GetStatComponent()->GetCurrentHealth() / Unit->GetStatComponent()->GetMaxHealth());
				if (LowHPPercent > UnitHPPercent)
				{
					LowHPPercent = UnitHPPercent;
					Candidates.Empty();
					Candidates.Add(Unit);
				}
				else if (LowHPPercent == UnitHPPercent)
				{
					Candidates.Add(Unit);
				}
			}
		}

		if (Candidates.Num() > 0)
		{
			float RandomIndex = FMath::RandRange(0, Candidates.Num() - 1);
			BestUnit = Candidates[RandomIndex];
		}

		return BestUnit;
	}
	}

	return nullptr;
}


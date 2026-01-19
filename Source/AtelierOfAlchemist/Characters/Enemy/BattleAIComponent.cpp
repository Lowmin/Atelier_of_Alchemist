#include "BattleAIComponent.h"
#include "../../BattleGameMode.h"

UBattleAIComponent::UBattleAIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBattleAIComponent::InitializeAI(UCharacterDataAsset* InDataAsset)
{
	EnemyData = Cast<UEnemyDataAsset>(InDataAsset);
}

void UBattleAIComponent::ProcessAITurn(ABattleUnit* InOwnerUnit)
{
	OwnerUnit = InOwnerUnit;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBattleAIComponent::DecideAction, 1.5f, false);
}

void UBattleAIComponent::DecideAction()
{
	if (!OwnerUnit || !EnemyData) return;

	ABattleGameMode* GM = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM) return;

	TArray<ABattleUnit*> PlayerUnits = GM->GetPlayerUnits();

	int32 SelectedSkillIndex = 0;
	ABattleUnit* SelectedTarget = nullptr;
	bool bPatternFound = false;

	// Priority Sort
	EnemyData->AIPatterns.Sort([](const FAIPattern& A, const FAIPattern& B) {
		return A.Priority > B.Priority;
		});

	for (const FAIPattern& Pattern : EnemyData->AIPatterns)
	{
		if (CheckCondition(Pattern))
		{
			SelectedTarget = FindTarget(Pattern.TargetType, PlayerUnits);
			if (SelectedTarget)
			{
				SelectedSkillIndex = Pattern.SkillIndex;
				bPatternFound = true;
				break;
			}
		}
	}

	if (!bPatternFound)
	{
		SelectedSkillIndex = 0;
		SelectedTarget = FindTarget(EAITargetType::Random, PlayerUnits);
	}

	if (SelectedTarget)
	{
		GM->ExecuteAIAction(OwnerUnit, SelectedSkillIndex, SelectedTarget);
	}
	else
	{
		GM->StartNextTurn();
	}
}

bool UBattleAIComponent::CheckCondition(const FAIPattern& Pattern)
{
	if (!OwnerUnit) return false;

	float HPRatio = 1.0f;

	if (UStatComponent* StatComp = OwnerUnit->FindComponentByClass<UStatComponent>())
	{
		HPRatio = StatComp->GetHPRatio();
	}

	switch (Pattern.Condition)
	{
	case EAICondition::Always:
		return true;
	case EAICondition::HP_LessThan:
		return HPRatio <= Pattern.ConditionValue;
	case EAICondition::Random_Chance:
		return FMath::FRand() <= Pattern.ConditionValue;
	default:
		return false;
	}
}

ABattleUnit* UBattleAIComponent::FindTarget(EAITargetType TargetType, const TArray<ABattleUnit*>& Candidates)
{
	TArray<ABattleUnit*> ValidCandidates;
	for (ABattleUnit* Unit : Candidates)
	{
		if (Unit)
		{
			float CurrentHP = 0.f;
			if (UStatComponent* Stat = Unit->FindComponentByClass<UStatComponent>())
			{
				CurrentHP = Stat->GetCurrentHealth();
			}

			if (CurrentHP > 0)
				ValidCandidates.Add(Unit);
		}
	}

	if (ValidCandidates.IsEmpty()) return nullptr;

	switch (TargetType)
	{
	case EAITargetType::Random:
		return ValidCandidates[FMath::RandRange(0, ValidCandidates.Num() - 1)];

	case EAITargetType::Weakest_HP:
		ValidCandidates.Sort([](const ABattleUnit& A, const ABattleUnit& B) {
			float HPA = 0.f; float HPB = 0.f;
			if (auto* StatA = A.FindComponentByClass<UStatComponent>()) HPA = StatA->GetCurrentHealth();
			if (auto* StatB = B.FindComponentByClass<UStatComponent>()) HPB = StatB->GetCurrentHealth();
			return HPA < HPB;
			});
		return ValidCandidates[0];

	case EAITargetType::Highest_HP:
		ValidCandidates.Sort([](const ABattleUnit& A, const ABattleUnit& B) {
			float HPA = 0.f; float HPB = 0.f;
			if (auto* StatA = A.FindComponentByClass<UStatComponent>()) HPA = StatA->GetCurrentHealth();
			if (auto* StatB = B.FindComponentByClass<UStatComponent>()) HPB = StatB->GetCurrentHealth();
			return HPA > HPB;
			});
		return ValidCandidates[0];

	default:
		return ValidCandidates[0];
	}
}
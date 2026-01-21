#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../DataAssets/EnemyDataAsset.h"
#include "../Battle/BattleUnit.h"
#include "BattleAIComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ATELIEROFALCHEMIST_API UBattleAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBattleAIComponent();

	TArray<FAIPattern> AIPatterns;
	TSet<int32> UsedPatternIndices;

	void RunAI();
	void DecideAction();
	bool CheckCondition(FAIPattern Pattern);
	ABattleUnit* FindTarget(EAITargetType TargetType);
};

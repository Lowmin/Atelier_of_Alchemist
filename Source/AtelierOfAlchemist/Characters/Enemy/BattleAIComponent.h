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

	void InitializeAI(UCharacterDataAsset* InDataAsset);
	void ProcessAITurn(ABattleUnit* InOwnerUnit);

protected:
	void DecideAction();
	bool CheckCondition(const FAIPattern& Pattern);
	ABattleUnit* FindTarget(EAITargetType TargetType, const TArray<ABattleUnit*>& Candidates);

private:
	UPROPERTY()
	ABattleUnit* OwnerUnit;

	UPROPERTY()
	UEnemyDataAsset* EnemyData;
};

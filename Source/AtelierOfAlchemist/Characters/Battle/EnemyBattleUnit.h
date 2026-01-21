#pragma once

#include "CoreMinimal.h"
#include "BattleUnit.h"
#include "EnemyBattleUnit.generated.h"

/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API AEnemyBattleUnit : public ABattleUnit
{
	GENERATED_BODY()
	
public:
	AEnemyBattleUnit();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateStatusHp(float CurrentHp, float MaxHp);
};

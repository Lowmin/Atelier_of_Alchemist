#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InventoryManagerSubsystem.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "BattleGameMode.generated.h"

class ABattleUnit;
class USkillDataAsset;
class AAoABattleController;

UENUM(BlueprintType)
enum class EBattleState : uint8
{
	Setup,
	TurnCalculation,
	ProcessTurn,
	PlayerTurn,
	EnemyTurn,
	ActionRunning,
	TurnEnd,
	Win,
	Lose
};

UCLASS()
class ATELIEROFALCHEMIST_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABattleGameMode();
	virtual void BeginPlay() override;

	void StartBattle();

	void ProcessNextTurn();

	void ExecuteAction(ABattleUnit* SourceUnit, ABattleUnit* TargetUnit, USkillDataAsset* SkillAsset);

	void OnUnitDead(ABattleUnit* Unit);

	void PlayBattleLoopSequence();

	TArray<ABattleUnit*> GetAllUnits() const { return AllUnits; }
	ABattleUnit* GetCurrentUnit() const { return CurrentActiveUnit; }

protected:
	void SetBattleState(EBattleState NewState);

	void SpawnPlayerUnits();

	void SpawnEnemyUnits();

	void CalculateTurnOrder();

	void CheckBattleResult();

	void EndBattle(bool bIsPlayerWin);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle State")
	EBattleState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Data")
	TArray<ABattleUnit*> AllUnits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Data")
	TArray<ABattleUnit*> TurnQueue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Data")
	TObjectPtr<ABattleUnit> CurrentActiveUnit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Data")
	TArray<FInventorySlotStruct> BattleRewards;

	UPROPERTY(EditDefaultsOnly, Category = "Sequences")
	TObjectPtr<ULevelSequence> BattleLoopSequenceAsset;

	UPROPERTY()
	TObjectPtr<ALevelSequenceActor> LoopSequenceActor;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LoopSequencePlayer;
};
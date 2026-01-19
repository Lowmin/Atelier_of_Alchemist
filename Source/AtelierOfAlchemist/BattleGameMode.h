#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGameMode.generated.h"

class UBattleMainLayout;
class ABattleSpawnPoint;
class ACharacterBase;
class ABattleUnit;
class UBattleManagerSubsystem;
class UGuildMemberManagerSubsystem;
class UCharacterDataAsset;

UCLASS()
class ATELIEROFALCHEMIST_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void ProcessPlayerAction(int32 ActionIndex);
	void ProcessSkillSelection(int32 SkillSlotIndex);
	void UndoLastAction();

	void StartBattle();
	void StartNextTurn();
	void TurnEnd();

	void ExecutePlayerSkill(int32 SkillIndex);
	void ExecuteAIAction(ABattleUnit* Attacker, int32 SkillIndex, ABattleUnit* Target);

	const TArray<ABattleUnit*>& GetAllUnits() const { return AllUnits; }
	const TObjectPtr<ABattleUnit>& GetCurrentUnit() const { return CurrentUnit; }

	TArray<ABattleUnit*> GetPlayerUnits() const;

protected:
	void CalculateTurnOrder();
	void UpdateTurnWidget();

	void ExecuteRunAway();
	void ShowSkillListUI();

	void FindEnemySpawnPoints();
	void SpawnPlayerParty();
	void SpawnEnemyParty();
	void InitializeBattleUI();

	UBattleManagerSubsystem* GetBattleManagerSubsystem() const;
	UGuildMemberManagerSubsystem* GetGuildMemberManagerSubsystem() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle|Units")
	TArray<ABattleUnit*> AllUnits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle|Units")
	TArray<ABattleUnit*> TurnQueue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle|Units")
	TObjectPtr<ABattleUnit> CurrentUnit;

	UPROPERTY()
	TMap<int32, AActor*> EnemySpawnPointMap;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|UI")
	TSubclassOf<UBattleMainLayout> MainLayoutClass;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|UI")
	TObjectPtr<UBattleMainLayout> MainLayoutInstance;
};
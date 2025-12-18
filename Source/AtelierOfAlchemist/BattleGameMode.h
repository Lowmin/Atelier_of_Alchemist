// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGameMode.generated.h"

class UBattleMainLayout;
class ABattleSpawnPoint;
class ACharacterBase;
class UBattleManagerSubsystem;
class UGuildMemberManagerSubsystem;
class ABattleUnit;

/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// --- [Life Cycle] ---
	virtual void BeginPlay() override;

	// --- [Input Interface] (UI나 컨트롤러에서 호출) ---
	void ProcessPlayerAction(int32 ActionIndex); // 이름 변경: PlayerAction -> ProcessPlayerAction
	void ProcessSkillSelection(int32 SkillSlotIndex);
	void UndoLastAction(); // 이름 변경: Undo -> UndoLastAction

	// --- [Battle Flow Control] ---
	void StartBattle();
	void StartNextTurn();

	// 적 AI가 호출하거나 턴 종료 시 호출될 함수
	void ExecuteEnemyTurn();

	const TArray<ABattleUnit*>& GetAllUnits() const { return AllUnits; };
	const TObjectPtr<ACharacterBase>& GetCurrentUnit() const { return CurrentUnit; };


protected:
	void CalculateTurnOrder();
	void UpdateTurnWidget();
	void CheckBattleEndCondition();

	void ExecuteAttack();
	void ExecuteSkill(int32 SkillIndex);
	void ExecuteRunAway();
	void ShowSkillSelectionUI();

	void FindEnemySpawnPoints();
	void SpawnPlayerParty();
	void SpawnEnemyParty();
	void InitializeBattleUI();

	UBattleManagerSubsystem* GetBattleManagerSubsystem() const;
	UGuildMemberManagerSubsystem* GetGuildMemberManagerSubsystem() const;

protected:
	// 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle|Units")
	TArray<ABattleUnit*> AllUnits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle|Units")
	TArray<ACharacterBase*> TurnQueue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle|Units")
	TObjectPtr<ACharacterBase> CurrentUnit;

	UPROPERTY()
	TMap<int32, AActor*> EnemySpawnPointMap;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|UI")
	TSubclassOf<UBattleMainLayout> MainLayoutClass;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|UI")
	TObjectPtr<UBattleMainLayout> MainLayoutInstance;
};
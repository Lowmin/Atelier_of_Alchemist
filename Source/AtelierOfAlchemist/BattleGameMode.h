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
	virtual void BeginPlay() override;

	void ProcessPlayerAction(int32 ActionIndex);
	void ProcessSkillSelection(int32 SkillSlotIndex);
	void UndoLastAction();

	void StartBattle();
	void StartNextTurn();
	void TurnEnd();

	const TArray<ABattleUnit*>& GetAllUnits() const { return AllUnits; };
	const TObjectPtr<ABattleUnit>& GetCurrentUnit() const { return CurrentUnit; };


protected:
	void CalculateTurnOrder();
	void UpdateTurnWidget();

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
	// µ•¿Ã≈Õ
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
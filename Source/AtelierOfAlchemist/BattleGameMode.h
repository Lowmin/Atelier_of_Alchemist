// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGameMode.generated.h"

class UBattleMainLayout;
class ABattleSpawnPoint;
class ACharacterBase;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	// 입력 처리
	void PlayerAction(int32 ActionIndex);
	void SkillSelect(int32 SkillSlotIndex);
	void Undo();

protected:
	// UI
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBattleMainLayout> MainLayoutClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UBattleMainLayout> MainLayoutInstance;

	// 유닛
	UPROPERTY()
	TArray<ACharacterBase*> AllUnits;

	UPROPERTY()
	TArray<ACharacterBase*> TurnQueue;

	UPROPERTY()
	TObjectPtr<ACharacterBase> CurrentUnit;

	// 스폰
	UPROPERTY()
	TMap<int32, AActor*> EnemySpawnPoints;

	void FindEnemySpawnPoints();
	void EnemySpawn();
	void PartySpawn();
	void InitPartyUI();

public:
	void StartBattleIntro();

	void OnAttack();
	void OnSkill();
	void UseSkill(int32 SkillIndex);
	void OnTarget(ACharacterBase* Target);
	void RunAway();

	void StartBattle();
	void CalculateTurn();
	void StartNextTurn();
	void UpdateTurnWidget();
};

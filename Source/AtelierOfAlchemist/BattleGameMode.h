// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGameMode.generated.h"

class UBattleMainLayout;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void PlayerAction(int32 ActionIndex);
	void SkillSelect(int32 SkillSlotIndex);
	void Undo();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBattleMainLayout> MainLayoutClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UBattleMainLayout> MainLayoutInstance;

	void PartySpawn();
	void EnemySpawn();

public:
	void Attack();
	void OpenSkillUI();
	void UseSkill();
	void RunAway();

	void StartBattle();
	void StartNextTurn();
	void ExecuteEnemyTurn();
};

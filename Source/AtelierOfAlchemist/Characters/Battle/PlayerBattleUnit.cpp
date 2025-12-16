// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBattleUnit.h"
#include "../../BattleGameMode.h"

APlayerBattleUnit::APlayerBattleUnit()
{
	Type = ECharacterType::Player;
}

void APlayerBattleUnit::TurnStart()
{
	Super::TurnStart();

	if (ABattleGameMode* BattleGameMode = GetWorld()->GetAuthGameMode<ABattleGameMode>())
	{
		BattleGameMode->UndoLastAction();
	}
}

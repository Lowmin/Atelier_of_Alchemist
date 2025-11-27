// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManagerSubsystem.h"

void UBattleManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FieldToBattleMapName.Add(FName("Test"), FName("Map_Battle_Test"));
}

FName UBattleManagerSubsystem::GetBattleLevelName(FName CurrentFieldLevelName) const
{
	if (FieldToBattleMapName.Contains(CurrentFieldLevelName))
	{
		return FieldToBattleMapName[CurrentFieldLevelName];
	}

	return FName("Test");
}

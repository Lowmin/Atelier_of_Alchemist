// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UBattleManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FieldToBattleMapName.Add(FName("Test"), FName("Map_Battle_Test"));
}

void UBattleManagerSubsystem::SaveFieldLocation(APawn* PlayerPawn)
{
	if (PlayerPawn)
	{
		SavedPlayerLocation = PlayerPawn->GetActorLocation();
		SavedPlayerRotation = PlayerPawn->GetActorRotation();

		SavedFieldLevelName = FName(*UGameplayStatics::GetCurrentLevelName(this));

		IsBattle = true;
	}
}

bool UBattleManagerSubsystem::GetSavedFieldLocation(FVector& OutLoc, FRotator& OutRot)
{
	if (IsBattle)
	{
		OutLoc = SavedPlayerLocation;
		OutRot = SavedPlayerRotation;

		IsBattle = false;
		return true;
	}

	return false;
}

FName UBattleManagerSubsystem::GetBattleLevelName(FName CurrentFieldLevelName) const
{
	if (FieldToBattleMapName.Contains(CurrentFieldLevelName))
	{
		return FieldToBattleMapName[CurrentFieldLevelName];
	}

	return FName("Test");
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "BattleManagerSubsystem.h"
#include "GuildMemberManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerRuntimeData.h"
#include "DataAssets/CharacterDataAsset.h"
#include "Characters/Playable/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "UI/Battle/BattleMainLayout.h"

void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 스폰
	PartySpawn();
	EnemySpawn();

	// 레이아웃 세팅
	if (MainLayoutClass)
	{
		MainLayoutInstance = CreateWidget<UBattleMainLayout>(GetWorld(), MainLayoutClass);
		if (MainLayoutInstance)
		{
			MainLayoutInstance->AddToViewport();
			MainLayoutInstance->ShowBattleUI();
		}
	}

	StartBattle();
}

void ABattleGameMode::PlayerAction(int32 ActionIndex)
{
	switch(ActionIndex)
	{
	case 0:
		Attack();
	case 1:
		OpenSkillUI();
	case 2:
		RunAway();
	}
}

void ABattleGameMode::SkillSelect(int32 SkillSlotIndex)
{

}

void ABattleGameMode::PartySpawn()
{
	UBattleManagerSubsystem* BattleManager = GetGameInstance()->GetSubsystem<UBattleManagerSubsystem>();
	UGuildMemberManagerSubsystem* GuildManager = GetGameInstance()->GetSubsystem<UGuildMemberManagerSubsystem>();

	const TArray<FName>& PartyIDs = GuildManager->GetPartyMemberIDs();
	TArray<AActor*> SpawnPoints;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "PlayerSpawn", SpawnPoints);

	for (int32 i = 0; i < PartyIDs.Num(); ++i)
	{
		if (i >= SpawnPoints.Num()) break;

		UPlayerRuntimeData* MemberData = GuildManager->GetPlayerRuntimeData(PartyIDs[i]);
		UCharacterDataAsset* DataAsset = MemberData->GetCharacterDataAsset();

		UClass* CharacterClass = DataAsset->BP_Character.LoadSynchronous();
		UE_LOG(LogTemp, Warning, TEXT("Load Character Class..."));
		if (CharacterClass)
		{
			ACharacter* CharacterData = Cast<ACharacter>(CharacterClass->GetDefaultObject());
			float SpawnHeight = CharacterData->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

			FVector SpawnLocation = SpawnPoints[i]->GetActorLocation();
			SpawnLocation.Z += SpawnHeight;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			APlayerCharacter* NewUnit = GetWorld()->SpawnActor<APlayerCharacter>(
				CharacterClass,
				SpawnLocation,
				SpawnPoints[i]->GetActorRotation(),
				SpawnParams
			);

			if (NewUnit)
			{
				UE_LOG(LogTemp, Warning, TEXT("Spawned Party Member: %s"), *DataAsset->CharacterName.ToString());
			}
		}
	}
}

void ABattleGameMode::EnemySpawn()
{

}

void ABattleGameMode::Undo()
{
	MainLayoutInstance->ShowBattleUI();
}

void ABattleGameMode::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack!"));
	MainLayoutInstance->ShowTargetUI();
}

void ABattleGameMode::OpenSkillUI()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Skill List"));
	MainLayoutInstance->ShowSkillUI();
}

void ABattleGameMode::UseSkill()
{

}

void ABattleGameMode::RunAway()
{

}

void ABattleGameMode::StartBattle()
{

}

void ABattleGameMode::StartNextTurn()
{

}

void ABattleGameMode::ExecuteEnemyTurn()
{

}

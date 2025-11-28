// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "BattleManagerSubsystem.h"
#include "GuildMemberManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerRuntimeData.h"
#include "DataAssets/CharacterDataAsset.h"
#include "Characters/Playable/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"

void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();
	PartySpawn();
	EnemySpawn();
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

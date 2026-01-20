#include "BattleGameMode.h"
#include "GuildMemberManagerSubsystem.h"
#include "BattleManagerSubsystem.h"
#include "Characters/Battle/BattleUnit.h"
#include "Characters/Enemy/EnemySymbol.h"
#include "Kismet/GameplayStatics.h"

ABattleGameMode::ABattleGameMode()
{
	CurrentState = EBattleState::Setup;
}

void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	StartBattle();
}

void ABattleGameMode::StartBattle()
{
	SpawnPlayerUnits();
	SpawnEnemyUnits();

	for (ABattleUnit* Unit : AllUnits)
	{
		if (Unit)
		{
			UE_LOG(LogTemp, Log, TEXT("Spawned: %s (Location: %s)"), *Unit->GetName(), *Unit->GetActorLocation().ToString());
		}
	}
}

void ABattleGameMode::ExecuteAction(ABattleUnit* SourceUnit, ABattleUnit* TargetUnit, USkillDataAsset* SkillAsset)
{
}

void ABattleGameMode::SetBattleState(EBattleState NewState)
{
}

void ABattleGameMode::SpawnPlayerUnits()
{
	UGuildMemberManagerSubsystem* GuildManager = GetGameInstance()->GetSubsystem<UGuildMemberManagerSubsystem>();
	
	TArray<AActor*> PlayerSpawnPoints;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Spawn_Player"), PlayerSpawnPoints);

	const TArray<FName>& PartyIDs = GuildManager->GetPartyMemberIDs();

	for (int32 i = 0; i < PartyIDs.Num(); i++)
	{
		if (!PlayerSpawnPoints.IsValidIndex(i)) break;

		FName MemberID = PartyIDs[i];
		UPlayerRuntimeData* RuntimeData = GuildManager->GetPlayerRuntimeData(MemberID);
		if (RuntimeData)
		{
			FVector SpawnLocation = PlayerSpawnPoints[i]->GetActorLocation();
			FRotator SpawnRotation = PlayerSpawnPoints[i]->GetActorRotation();

			ABattleUnit* NewUnit = GetWorld()->SpawnActor<ABattleUnit>(ABattleUnit::StaticClass(), SpawnLocation, SpawnRotation);
			if (NewUnit)
			{
				NewUnit->InitializeAsPlayerUnit(RuntimeData);
				AllUnits.Add(NewUnit);
			}
		}
	}
}

void ABattleGameMode::SpawnEnemyUnits()
{
	UBattleManagerSubsystem* BattleManager = GetGameInstance()->GetSubsystem<UBattleManagerSubsystem>();

	const TArray<FEnemySpawnInfo>& EnemyList = BattleManager->GetCurrentEnemyParty();
	if (EnemyList.IsEmpty()) return;

	TArray<AActor*> EnemySpawnPoints;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Spawn_Enemy"), EnemySpawnPoints);

	for (int32 i = 0; i < EnemyList.Num(); i++)
	{
		if (!EnemySpawnPoints.IsValidIndex(i)) break;

		FVector SpawnLocation = EnemySpawnPoints[i]->GetActorLocation();
		FRotator SpawnRotation = EnemySpawnPoints[i]->GetActorRotation();

		ABattleUnit* NewUnit = GetWorld()->SpawnActor<ABattleUnit>(ABattleUnit::StaticClass(), SpawnLocation, SpawnRotation);
		if (NewUnit)
		{
			NewUnit->InitializeAsEnemyUnit(EnemyList[i].EnemyData, EnemyList[i].Level);
			AllUnits.Add(NewUnit);
		}
	}
}

void ABattleGameMode::CalculateTurnOrder()
{
}

void ABattleGameMode::ProcessNextTurn()
{
}

void ABattleGameMode::CheckBattleResult()
{
}

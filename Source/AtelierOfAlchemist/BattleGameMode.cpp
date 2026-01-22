#include "BattleGameMode.h"
#include "GuildMemberManagerSubsystem.h"
#include "BattleManagerSubsystem.h"
#include "AoABattleController.h"
#include "Characters/Battle/BattleUnit.h"
#include "Characters/Enemy/EnemySymbol.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Enemy/BattleAIComponent.h"
#include "Characters/StatComponent.h"
#include "DataAssets/SkillDataAsset.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Engine/DamageEvents.h"

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
	CalculateTurnOrder();
	PlayBattleLoopSequence();
}

void ABattleGameMode::ExecuteAction(ABattleUnit* SourceUnit, ABattleUnit* TargetUnit, USkillDataAsset* SkillAsset)
{
	if (!SourceUnit || !TargetUnit || !SkillAsset) return;

	SetBattleState(EBattleState::ActionRunning);

	float AttackPower = SourceUnit->GetStatComponent()->GetAttackPower();
	float DefensePower = TargetUnit->GetStatComponent()->GetDefense();
	float SkillRatio = SkillAsset->Power;
	float RawDamage = (AttackPower * SkillRatio) - DefensePower;
	float FinalDamage = FMath::Max(RawDamage, 1.0f);

	SourceUnit->StartAttack(TargetUnit, FinalDamage, SkillAsset);
}

void ABattleGameMode::OnUnitDead(ABattleUnit* Unit)
{
	if (!Unit) return;

	if (Unit->Type == ECharacterType::Enemy)
	{
		UBattleManagerSubsystem* BattleManager = GetGameInstance()->GetSubsystem<UBattleManagerSubsystem>();

		/*if (BattleManager && Unit->EnemyDataAsset)
		{
			TArray<FInventorySlotStruct> Drops = BattleManager->GenerateDropItem(Unit->EnemyDataAsset);
			BattleRewards.Append(Drops);
		}*/
	}

	if (AllUnits.Contains(Unit))
	{
		AllUnits.Remove(Unit);
	}
	if (TurnQueue.Contains(Unit))
	{
		TurnQueue.Remove(Unit);
	}

	CheckBattleResult();
}

void ABattleGameMode::PlayBattleLoopSequence()
{
	if (!BattleLoopSequenceAsset) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	if (LoopSequencePlayer)
	{
		LoopSequencePlayer->PlayLooping();
		if (LoopSequenceActor)
		{
			PC->SetViewTargetWithBlend(LoopSequenceActor, 0.5f);
		}
		return;
	}

	ALevelSequenceActor* OutActor = nullptr;
	LoopSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		BattleLoopSequenceAsset,
		FMovieSceneSequencePlaybackSettings(),
		OutActor
	);

	if (LoopSequencePlayer && OutActor)
	{
		LoopSequenceActor = OutActor;
		LoopSequencePlayer->PlayLooping();
		PC->SetViewTargetWithBlend(OutActor, 0.0f);
	}
}

void ABattleGameMode::SetBattleState(EBattleState NewState)
{
	CurrentState = NewState;
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
			UCharacterDataAsset* DataAsset = RuntimeData->GetCharacterDataAsset();
			UClass* SpawnClass = DataAsset->CharacterBP.LoadSynchronous();

			if (SpawnClass)
			{
				FVector SpawnLocation = PlayerSpawnPoints[i]->GetActorLocation();
				FRotator SpawnRotation = PlayerSpawnPoints[i]->GetActorRotation();

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				ABattleUnit* NewUnit = GetWorld()->SpawnActor<ABattleUnit>(SpawnClass, SpawnLocation, SpawnRotation, SpawnParams);

				if (NewUnit)
				{
					float HalfHeight = NewUnit->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

					FVector CorrectLocation = SpawnLocation;
					CorrectLocation.Z += HalfHeight;

					NewUnit->SetActorLocation(CorrectLocation);

					NewUnit->InitializeAsPlayerUnit(RuntimeData);
					AllUnits.Add(NewUnit);
				}
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

		UEnemyDataAsset* Data = EnemyList[i].EnemyData;
		if (Data)
		{
			UClass* SpawnClass = Data->CharacterBP.LoadSynchronous();

			if (SpawnClass)
			{
				FVector SpawnLocation = EnemySpawnPoints[i]->GetActorLocation();
				FRotator SpawnRotation = EnemySpawnPoints[i]->GetActorRotation();

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				ABattleUnit* NewUnit = GetWorld()->SpawnActor<ABattleUnit>(SpawnClass, SpawnLocation, SpawnRotation, SpawnParams);

				if (NewUnit)
				{
					float HalfHeight = NewUnit->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
					FVector CorrectLocation = SpawnLocation;
					CorrectLocation.Z += HalfHeight;
					NewUnit->SetActorLocation(CorrectLocation);

					NewUnit->InitializeAsEnemyUnit(Data, EnemyList[i].Level);
					AllUnits.Add(NewUnit);
				}
			}
		}
	}
}

void ABattleGameMode::CalculateTurnOrder()
{
	TurnQueue.Empty();

	for (ABattleUnit* Unit : AllUnits)
	{
		if (Unit && Unit->GetStatComponent() && Unit->GetStatComponent()->GetCurrentHealth() > 0)
		{
			TurnQueue.Add(Unit);
		}
	}

	TurnQueue.Sort([](const ABattleUnit& A, const ABattleUnit& B)
		{
			float SpeedA = A.GetStatComponent()->GetSpeed();
			float SpeedB = B.GetStatComponent()->GetSpeed();
			return SpeedA > SpeedB;
		});

	SetBattleState(EBattleState::ProcessTurn);
	ProcessNextTurn();
}

void ABattleGameMode::ProcessNextTurn()
{
	if (TurnQueue.IsEmpty())
	{
		CalculateTurnOrder();
		return;
	}

	CurrentActiveUnit = TurnQueue[0];
	TurnQueue.RemoveAt(0);

	if (CurrentActiveUnit->GetStatComponent()->GetCurrentHealth() <= 0)
	{
		ProcessNextTurn();
		return;
	}

	if (CurrentActiveUnit->IsPlayerTeam())
	{
		SetBattleState(EBattleState::PlayerTurn);

		AAoABattleController* BattleController = Cast<AAoABattleController>(GetWorld()->GetFirstPlayerController());
		if (BattleController)
		{
			BattleController->StartPlayerTurn();
		}
	}
	else
	{
		SetBattleState(EBattleState::EnemyTurn);

		UBattleAIComponent* AI = CurrentActiveUnit->FindComponentByClass<UBattleAIComponent>();
		if (AI)
		{
			AI->RunAI();
		}
		else
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
				{
					ProcessNextTurn();
				}, 1.0f, false);
		}
	}
}

void ABattleGameMode::CheckBattleResult()
{
	int32 PlayerCount = 0;
	int32 EnemyCount = 0;

	for (ABattleUnit* Unit : AllUnits)
	{
		if (Unit && Unit->IsPlayerTeam())
		{
			PlayerCount++;
		}
		else if (Unit && !Unit->IsPlayerTeam())
		{
			EnemyCount++;
		}
	}

	if (EnemyCount == 0)
	{
		EndBattle(true);
	}
	else if (PlayerCount == 0)
	{
		EndBattle(false);
	}
}

void ABattleGameMode::EndBattle(bool bIsPlayerWin)
{
	if (bIsPlayerWin)
	{
		SetBattleState(EBattleState::Win);

		UBattleManagerSubsystem* BattleManager = GetGameInstance()->GetSubsystem<UBattleManagerSubsystem>();
		if (BattleManager)
		{
			//BattleManager->EndBattleWithVictory(BattleRewards);
		}
	}
	else
	{
		SetBattleState(EBattleState::Lose);
	}
}
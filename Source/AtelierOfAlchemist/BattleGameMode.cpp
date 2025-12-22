#include "BattleGameMode.h"

#include "BattleManagerSubsystem.h"
#include "GuildMemberManagerSubsystem.h"
#include "AoABattleController.h"

#include "DataAssets/CharacterDataAsset.h"
#include "DataAssets/EnemyPartyDataAsset.h"
#include "DataAssets/SkillDataAsset.h"
#include "PlayerRuntimeData.h"
#include "Object/BattleSpawnPoint.h"

#include "Characters/CharacterBase.h"
#include "Characters/Battle/BattleUnit.h" 

#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "UI/Battle/BattleMainLayout.h"


void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	FindEnemySpawnPoints();
	SpawnPlayerParty();
	SpawnEnemyParty();
	InitializeBattleUI();

	StartBattle();
}


void ABattleGameMode::ProcessPlayerAction(int32 ActionIndex)
{
	switch (ActionIndex)
	{
	case 0: ExecuteAttack(); break;
	case 1: ShowSkillSelectionUI(); break;
	case 2: ExecuteRunAway(); break;
	default: break;
	}
}

void ABattleGameMode::ProcessSkillSelection(int32 SkillSlotIndex)
{
	MainLayoutInstance->HideBattleUI();
	ExecuteSkill(SkillSlotIndex);
}

void ABattleGameMode::UndoLastAction()
{
	if (MainLayoutInstance)
	{
		MainLayoutInstance->ShowBattleUI();
	}
}

void ABattleGameMode::StartBattle()
{
	CalculateTurnOrder();
	StartNextTurn();
}

void ABattleGameMode::CalculateTurnOrder()
{
	TurnQueue.Empty();

	for (ABattleUnit* Unit : AllUnits)
	{
		if (Unit && Unit->GetCurHealth() > 0)
		{
			TurnQueue.Add(Unit);
		}
	}

	TurnQueue.Sort([](const ACharacterBase& A, const ACharacterBase& B)
		{
			return A.GetSpeed() > B.GetSpeed();
		});

	UpdateTurnWidget();
}

void ABattleGameMode::StartNextTurn()
{
	if (TurnQueue.IsEmpty())
	{
		CalculateTurnOrder();
	}

	if (TurnQueue.IsEmpty()) return;

	CurrentUnit = TurnQueue[0];
	TurnQueue.RemoveAt(0);
	UpdateTurnWidget();

	if (CurrentUnit)
	{
		FString UnitName = "Unknown";
		if (UCharacterDataAsset* Data = CurrentUnit->GetCharacterData())
		{
			UnitName = Data->CharacterName.ToString();
		}
		UE_LOG(LogTemp, Log, TEXT(">>> Start Turn: %s"), *UnitName);

		if (CurrentUnit->Type == ECharacterType::Player)
		{
			if (AAoABattleController* BattleController = Cast<AAoABattleController>(GetWorld()->GetFirstPlayerController()))
			{
				if (MainLayoutInstance) MainLayoutInstance->ShowBattleUI();

				BattleController->SetInputMode_Main();
			}
		}

		if (ABattleUnit* BattleUnit = Cast<ABattleUnit>(CurrentUnit))
		{
			BattleUnit->TurnStart();
		}
	}
}

void ABattleGameMode::TurnEnd()
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ABattleGameMode::StartNextTurn, 1.0f, false);
}

void ABattleGameMode::UpdateTurnWidget()
{
	if (MainLayoutInstance)
	{
		MainLayoutInstance->UpdateTurnSlotBar(TurnQueue);
	}
}

void ABattleGameMode::ExecuteAttack()
{
	ExecuteSkill(0);
}

void ABattleGameMode::ShowSkillSelectionUI()
{
	if (MainLayoutInstance)
	{
		MainLayoutInstance->ShowSkillUI();
	}
}

void ABattleGameMode::ExecuteSkill(int32 SkillIndex)
{
	ABattleUnit* BattleUnit = Cast<ABattleUnit>(CurrentUnit);
	if (!BattleUnit) return;

	USkillDataAsset* SkillData = BattleUnit->GetSkill(SkillIndex);
	if (!SkillData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BattleGameMode -> ExecuteSkill] No Skill Data."));
		UndoLastAction();
		return;
	}

	MainLayoutInstance->HideBattleUI();

	AAoABattleController* PC = Cast<AAoABattleController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->StartTargetingMode(SkillData);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[BattleGameMode -> ExecuteSkill] No BattleController."));
	}
}

void ABattleGameMode::ExecuteRunAway()
{
	if (MainLayoutInstance)
	{
		MainLayoutInstance->HideBattleUI();
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		float FadeOutDuration = 1.0f;
		if (PlayerController->PlayerCameraManager)
		{
			PlayerController->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, FadeOutDuration, FLinearColor::Black, false, true);
		}

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
			{
				if (UBattleManagerSubsystem* BattleManager = GetBattleManagerSubsystem())
				{
					FName ReturnLevel = BattleManager->SavedFieldLevelName;
					if (!ReturnLevel.IsNone())
					{
						UGameplayStatics::OpenLevel(this, ReturnLevel);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("[BattleGameMode -> ExecuteRunAway] No Level."));
					}
				}
			}, FadeOutDuration, false);
	}
}

void ABattleGameMode::FindEnemySpawnPoints()
{
	TArray<AActor*> FoundPoints;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "EnemySpawn", FoundPoints);

	EnemySpawnPointMap.Empty();

	for (AActor* Actor : FoundPoints)
	{
		if (ABattleSpawnPoint* Point = Cast<ABattleSpawnPoint>(Actor))
		{
			if (EnemySpawnPointMap.Contains(Point->SpawnIndex))
			{
				UE_LOG(LogTemp, Error, TEXT("[Spawn] Duplicate Index Found: %d at %s"), Point->SpawnIndex, *Point->GetName());
			}
			else
			{
				EnemySpawnPointMap.Add(Point->SpawnIndex, Point);
			}
		}
	}
}

void ABattleGameMode::SpawnPlayerParty()
{
	UGuildMemberManagerSubsystem* GuildManager = GetGuildMemberManagerSubsystem();
	if (!GuildManager) return;

	const TArray<FName>& PartyIDs = GuildManager->GetPartyMemberIDs();
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "PlayerSpawn", SpawnPoints);

	for (int32 i = 0; i < PartyIDs.Num(); ++i)
	{
		if (i >= SpawnPoints.Num()) break;

		UPlayerRuntimeData* MemberData = GuildManager->GetPlayerRuntimeData(PartyIDs[i]);
		if (!MemberData) continue;

		UCharacterDataAsset* DataAsset = MemberData->GetCharacterDataAsset();
		if (!DataAsset) continue;

		UClass* CharacterClass = DataAsset->BP_Character.LoadSynchronous();
		if (CharacterClass)
		{
			ACharacter* CDO = Cast<ACharacter>(CharacterClass->GetDefaultObject());
			float HalfHeight = CDO ? CDO->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() : 88.0f;

			FVector SpawnLoc = SpawnPoints[i]->GetActorLocation();
			SpawnLoc.Z += HalfHeight;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			ABattleUnit* NewUnit = GetWorld()->SpawnActor<ABattleUnit>(
				CharacterClass, SpawnLoc, SpawnPoints[i]->GetActorRotation(), SpawnParams);

			if (NewUnit)
			{
				AllUnits.Add(NewUnit);
				if (UStatComponent* StatComp = NewUnit->GetStatComponent())
				{
					StatComp->Initialize(MemberData);
				}
			}
		}
	}
}

void ABattleGameMode::SpawnEnemyParty()
{
	UBattleManagerSubsystem* BattleManager = GetBattleManagerSubsystem();

	if (!BattleManager)
	{
		return;
	}

	if (!BattleManager->EnemyPartyData.IsValid())
	{
		return;
	}

	UEnemyPartyDataAsset* EnemyPartyData = BattleManager->EnemyPartyData.LoadSynchronous();
	if (!EnemyPartyData)
	{
		return;
	}

	for (const FEnemySpawnInfo& Info : EnemyPartyData->EnemyMembers)
	{
		AActor** SpawnPointPtr = EnemySpawnPointMap.Find(Info.SpawnIndex);

		AActor* SpawnPoint = *SpawnPointPtr;
		UCharacterDataAsset* EnemyData = Info.EnemyData.LoadSynchronous();

		UClass* EnemyClass = EnemyData->BP_Character.LoadSynchronous();

		ACharacter* CDO = Cast<ACharacter>(EnemyClass->GetDefaultObject());
		float HalfHeight = CDO ? CDO->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() : 88.0f;

		FVector SpawnLoc = SpawnPoint->GetActorLocation() + FVector(0, 0, HalfHeight + 2.0f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABattleUnit* NewEnemy = GetWorld()->SpawnActor<ABattleUnit>(
			EnemyClass, SpawnLoc, SpawnPoint->GetActorRotation(), SpawnParams);

		if (NewEnemy)
		{
			AllUnits.Add(NewEnemy);

			if (UStatComponent* StatComp = NewEnemy->GetStatComponent())
			{
				StatComp->InitializeFromEnemy(EnemyData);
			}
		}
	}
}

void ABattleGameMode::InitializeBattleUI()
{
	if (!MainLayoutClass) return;

	MainLayoutInstance = CreateWidget<UBattleMainLayout>(GetWorld(), MainLayoutClass);
	if (MainLayoutInstance)
	{
		MainLayoutInstance->AddToViewport();
		MainLayoutInstance->ShowBattleUI();

		if (UGuildMemberManagerSubsystem* GuildManager = GetGuildMemberManagerSubsystem())
		{
			TArray<UPlayerRuntimeData*> PartyDataList;
			const TArray<FName>& PartyIDs = GuildManager->GetPartyMemberIDs();

			for (const FName& ID : PartyIDs)
			{
				if (auto* Data = GuildManager->GetPlayerRuntimeData(ID))
				{
					PartyDataList.Add(Data);
				}
			}
			MainLayoutInstance->InitStatusSlot(PartyDataList);
		}
	}
}

UBattleManagerSubsystem* ABattleGameMode::GetBattleManagerSubsystem() const
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		return GameInstance->GetSubsystem<UBattleManagerSubsystem>();
	}
	return nullptr;
}

UGuildMemberManagerSubsystem* ABattleGameMode::GetGuildMemberManagerSubsystem() const
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		return GameInstance->GetSubsystem<UGuildMemberManagerSubsystem>();
	}
	return nullptr;
}
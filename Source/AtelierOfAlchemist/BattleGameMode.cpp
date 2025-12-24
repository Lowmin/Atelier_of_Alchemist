#include "BattleGameMode.h"
#include "BattleManagerSubsystem.h"
#include "GuildMemberManagerSubsystem.h"
#include "AoABattleController.h"
#include "DataAssets/CharacterDataAsset.h"
#include "DataAssets/EnemyPartyDataAsset.h"
#include "DataAssets/SkillDataAsset.h"
#include "PlayerRuntimeData.h"
#include "Object/BattleSpawnPoint.h"
#include "Characters/Battle/BattleUnit.h" 
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "UI/Battle/BattleMainLayout.h"
#include "DataAssets/SkillListComponent.h"

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
	case 1: ShowSkillListUI(); break;
	case 2: ExecuteRunAway(); break;
	default: break;
	}
}

void ABattleGameMode::ProcessSkillSelection(int32 SkillSlotIndex)
{
	if (MainLayoutInstance) MainLayoutInstance->HideBattleUI();
	ExecuteSkill(SkillSlotIndex);
}

void ABattleGameMode::UndoLastAction()
{
	if (MainLayoutInstance) MainLayoutInstance->ShowBattleUI();
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
	if (TurnQueue.IsEmpty()) CalculateTurnOrder();
	if (TurnQueue.IsEmpty()) return;

	CurrentUnit = TurnQueue[0];
	TurnQueue.RemoveAt(0);
	UpdateTurnWidget();

	if (CurrentUnit)
	{
		if (CurrentUnit->Type == ECharacterType::Player)
		{
			if (AAoABattleController* BattleController = Cast<AAoABattleController>(GetWorld()->GetFirstPlayerController()))
			{
				if (MainLayoutInstance) MainLayoutInstance->ShowBattleUI();
				BattleController->SetInputMode_Main();
			}
		}

		CurrentUnit->TurnStart();
	}
}

void ABattleGameMode::TurnEnd()
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ABattleGameMode::StartNextTurn, 1.0f, false);
}

void ABattleGameMode::UpdateTurnWidget()
{
	if (MainLayoutInstance) MainLayoutInstance->UpdateTurnSlotBar(TurnQueue);
}

void ABattleGameMode::ExecuteAttack()
{
	ExecuteSkill(0);
}

void ABattleGameMode::ShowSkillListUI()
{
	if (!CurrentUnit) return;

	if (MainLayoutInstance)
	{
		if (USkillListComponent* SkillComponent = CurrentUnit->GetSkillComponent())
		{
			MainLayoutInstance->InitSkillList(SkillComponent->GetSkillList());
			MainLayoutInstance->ShowSkillUI();
		}
	}

	if (AAoABattleController* BattleController = Cast<AAoABattleController>(GetWorld()->GetFirstPlayerController()))
	{
		BattleController->SetInputMode_Skill();
	}
}

void ABattleGameMode::ExecuteSkill(int32 SkillIndex)
{
	if (!CurrentUnit) return;

	USkillDataAsset* SkillData = nullptr;
	if (USkillListComponent* SkillComp = CurrentUnit->GetSkillComponent())
	{
		SkillData = SkillComp->GetSkillIndex(SkillIndex);
	}

	if (!SkillData)
	{
		UndoLastAction();
		return;
	}

	if (MainLayoutInstance) MainLayoutInstance->HideBattleUI();

	if (AAoABattleController* PC = Cast<AAoABattleController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->StartTargetingMode(SkillData);
	}
}

void ABattleGameMode::ExecuteRunAway()
{
	if (MainLayoutInstance) MainLayoutInstance->HideBattleUI();

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
			EnemySpawnPointMap.Add(Point->SpawnIndex, Point);
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
		if (!MemberData || !MemberData->GetCharacterDataAsset()) continue;

		UCharacterDataAsset* DataAsset = MemberData->GetCharacterDataAsset();
		UClass* CharacterClass = DataAsset->BP_Character.LoadSynchronous();

		if (CharacterClass)
		{
			ACharacter* CDO = Cast<ACharacter>(CharacterClass->GetDefaultObject());
			float HalfHeight = CDO ? CDO->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() : 88.0f;
			FVector SpawnLoc = SpawnPoints[i]->GetActorLocation() + FVector(0, 0, HalfHeight);

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			if (ABattleUnit* NewUnit = GetWorld()->SpawnActor<ABattleUnit>(CharacterClass, SpawnLoc, SpawnPoints[i]->GetActorRotation(), SpawnParams))
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
	if (!BattleManager || !BattleManager->CurrentEnemyPartyData) return;

	for (const FEnemySpawnInfo& Info : BattleManager->CurrentEnemyPartyData->EnemyMembers)
	{
		AActor** SpawnPointPtr = EnemySpawnPointMap.Find(Info.SpawnIndex);
		if (!SpawnPointPtr) continue;

		AActor* SpawnPoint = *SpawnPointPtr;
		if (!Info.EnemyData) continue;

		UClass* EnemyClass = Info.EnemyData->BP_Character.LoadSynchronous();
		if (!EnemyClass) continue;

		ACharacter* CDO = Cast<ACharacter>(EnemyClass->GetDefaultObject());
		float HalfHeight = CDO ? CDO->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() : 88.0f;
		FVector SpawnLoc = SpawnPoint->GetActorLocation() + FVector(0, 0, HalfHeight + 2.0f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		if (ABattleUnit* NewEnemy = GetWorld()->SpawnActor<ABattleUnit>(EnemyClass, SpawnLoc, SpawnPoint->GetActorRotation(), SpawnParams))
		{
			AllUnits.Add(NewEnemy);
			if (UStatComponent* StatComp = NewEnemy->GetStatComponent())
			{
				StatComp->InitializeFromEnemy(Info.EnemyData);
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
			for (const FName& ID : GuildManager->GetPartyMemberIDs())
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
	return GetGameInstance() ? GetGameInstance()->GetSubsystem<UBattleManagerSubsystem>() : nullptr;
}

UGuildMemberManagerSubsystem* ABattleGameMode::GetGuildMemberManagerSubsystem() const
{
	return GetGameInstance() ? GetGameInstance()->GetSubsystem<UGuildMemberManagerSubsystem>() : nullptr;
}
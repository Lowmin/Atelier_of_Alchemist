#include "BattleGameMode.h"
#include "BattleManagerSubsystem.h"
#include "GuildMemberManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerRuntimeData.h"
#include "DataAssets/CharacterDataAsset.h"
#include "DataAssets/EnemyPartyDataAsset.h"
#include "Characters/Playable/PlayerCharacter.h"
#include "Characters/CharacterBase.h"
#include "Characters/Enemy/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "UI/Battle/BattleMainLayout.h"
#include "Object/BattleSpawnPoint.h"

void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 스폰
	FindEnemySpawnPoints();
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
			InitPartyUI();
		}
	}

	StartBattle();
}

void ABattleGameMode::PlayerAction(int32 ActionIndex)
{
	switch(ActionIndex)
	{
	case 0:
		OnAttack();
		break;
	case 1:
		OnSkill();
		break;
	case 2:
		RunAway();
		break;
	}
}

void ABattleGameMode::SkillSelect(int32 SkillSlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("스킬 %d번 사용!"), SkillSlotIndex);
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
				AllUnits.Add(NewUnit);

				if (UStatComponent* StatComp = NewUnit->GetStatComponent())
				{
					StatComp->Initialize(MemberData);
				}
			}
		}
	}
}

void ABattleGameMode::InitPartyUI()
{
	if (!MainLayoutInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("InitPartyUI error! MainLayoutInstance."));
		return;
	}

	UGuildMemberManagerSubsystem* GuildMemberManagerSubsystem = GetGameInstance()->GetSubsystem<UGuildMemberManagerSubsystem>();
	if (!GuildMemberManagerSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("InitPartyUI error! GuildMemberManagerSubsystem."));
		return;
	}

	const TArray<FName>& PartyID = GuildMemberManagerSubsystem->GetPartyMemberIDs();

	TArray<UPlayerRuntimeData*> PartyRuntimeData;
	for (const FName& ID : PartyID)
	{
		if (UPlayerRuntimeData* Data = GuildMemberManagerSubsystem->GetPlayerRuntimeData(ID))
			PartyRuntimeData.Add(Data);
	}

	MainLayoutInstance->InitStatusSlot(PartyRuntimeData);
}

void ABattleGameMode::FindEnemySpawnPoints()
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "EnemySpawn", SpawnPoints);

	for (AActor* Actor : SpawnPoints)
	{
		if (ABattleSpawnPoint* Point = Cast<ABattleSpawnPoint>(Actor))
		{
			EnemySpawnPoints.Add(Point->SpawnIndex, Point);
		}
	}
}

void ABattleGameMode::EnemySpawn()
{
	UBattleManagerSubsystem* BattleManager = GetGameInstance()->GetSubsystem<UBattleManagerSubsystem>();
	if (!BattleManager) return;

	UEnemyPartyDataAsset* EnemyPartyData = BattleManager->EnemyPartyData.LoadSynchronous();

	if (!EnemyPartyData)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyPartyData does not exist."));
		return;
	}

	for (const FEnemySpawnInfo& EnemySpawnInfo : EnemyPartyData->EnemyMembers)
	{
		UCharacterDataAsset* EnemyData = EnemySpawnInfo.EnemyData.LoadSynchronous();

		if (AActor** SpawnPointPtr = EnemySpawnPoints.Find(EnemySpawnInfo.SpawnIndex))
		{
			AActor* SpawnPoint = *SpawnPointPtr;

			UClass* EnemyClass = EnemyData->BP_Character.LoadSynchronous();
			if (EnemyClass)
			{
				ACharacter* CDO = Cast<ACharacter>(EnemyClass->GetDefaultObject());
				float SpawnHeight = CDO ? CDO->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() : 88.f;
				FVector SpawnLoc = SpawnPoint->GetActorLocation() + FVector(0, 0, SpawnHeight + 2.0f);
				AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(
					EnemyClass, SpawnLoc, SpawnPoint->GetActorRotation());

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
	}
}

void ABattleGameMode::Undo()
{
	MainLayoutInstance->ShowBattleUI();
}

void ABattleGameMode::StartBattleIntro()
{
	TArray<AActor*> BattleCamera;
}

void ABattleGameMode::OnAttack()
{
	UseSkill(0);
}

void ABattleGameMode::OnSkill()
{
	MainLayoutInstance->ShowSkillUI();
}

void ABattleGameMode::UseSkill(int32 SkillIndex)
{
	USkillDataAsset* SkillData = CurrentUnit->GetSkill(SkillIndex);
	if (!SkillData)
	{
		StartNextTurn();
		return;
	}

	TArray<ACharacterBase*> Targets;

	for (ACharacterBase* Unit : AllUnits)
	{
		if (Unit && Unit->GetCurHealth() > 0 && Unit->Type == ECharacterType::Enemy)
		{
			Targets.Add(Unit);
		}
	}

	if (Targets.Num() > 0)
	{
		CurrentUnit->BattleAction_UseSkill(SkillData, Targets);
	}

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ABattleGameMode::StartNextTurn, 1.5f, false);
}

void ABattleGameMode::OnTarget(ACharacterBase* Target)
{

}

void ABattleGameMode::RunAway()
{
	UE_LOG(LogTemp, Warning, TEXT("Run"));
}

void ABattleGameMode::StartBattle()
{
	CalculateTurn();
	StartNextTurn();
}

void ABattleGameMode::CalculateTurn()
{
	TurnQueue.Empty();

	for (ACharacterBase* Unit : AllUnits)
	{
		if (Unit->GetCurHealth() >= 0)
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
		CalculateTurn();
	}


	CurrentUnit = TurnQueue[0];

	FString Name = CurrentUnit->GetCharacterData()->CharacterName.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s turn!"), *Name);

	TurnQueue.RemoveAt(0);

	UpdateTurnWidget();
}

void ABattleGameMode::UpdateTurnWidget()
{
	if (MainLayoutInstance)
	{
		MainLayoutInstance->UpdateTurnSlotBar(TurnQueue);
	}
}
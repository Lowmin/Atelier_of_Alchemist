#include "BattleManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "DataAssets/EnemyPartyDataAsset.h"

void UBattleManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FieldToBattleMapName.Add(FName("Map_Field_Forest"), FName("Map_Battle_Forest"));
	FieldToBattleMapName.Add(FName("Test"), FName("Map_Battle_Test"));
}

void UBattleManagerSubsystem::StartBattle(AActor* PlayerActor, AActor* EnemyActor, UEnemyPartyDataAsset* InPartyData, UObject* WorldContextObject)
{
	if (!PlayerActor || !InPartyData) return;

	this->CurrentEnemyPartyData = InPartyData;

	if (EnemyActor)
	{
		this->LastEncounterSymbolName = EnemyActor->GetFName();
	}

	SaveFieldLocation(Cast<APawn>(PlayerActor));

	FString CurrentLevelString = UGameplayStatics::GetCurrentLevelName(WorldContextObject);
	FName BattleLevelName = GetBattleLevelName(FName(*CurrentLevelString));

	UGameplayStatics::OpenLevel(WorldContextObject, BattleLevelName);
}

void UBattleManagerSubsystem::SaveFieldLocation(APawn* PlayerPawn)
{
	if (PlayerPawn)
	{
		SavedPlayerLocation = PlayerPawn->GetActorLocation();
		SavedPlayerRotation = PlayerPawn->GetActorRotation();
		SavedFieldLevelName = FName(*UGameplayStatics::GetCurrentLevelName(this));
		bIsBattle = true;
	}
}

bool UBattleManagerSubsystem::GetSavedFieldLocation(FVector& OutLoc, FRotator& OutRot)
{
	if (bIsBattle)
	{
		OutLoc = SavedPlayerLocation;
		OutRot = SavedPlayerRotation;

		bIsBattle = false;
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

	UE_LOG(LogTemp, Warning, TEXT("[BattleManager] No battle map mapped for %s. Loading Test Map."), *CurrentFieldLevelName.ToString());
	return FName("Map_Battle_Test");
}

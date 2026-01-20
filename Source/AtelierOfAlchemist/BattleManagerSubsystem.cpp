#include "BattleManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "DataAssets/EnemyPartyDataAsset.h"

void UBattleManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FieldToBattleMapName.Add(FName("Map_Field_Forest"), FName("Map_Battle_Forest"));
	FieldToBattleMapName.Add(FName("Test"), FName("Map_Battle_Test"));
}

void UBattleManagerSubsystem::StartBattle(AActor* PlayerActor, AActor* EnemyActor, const TArray<FEnemySpawnInfo>& InEnemyParty, UObject* WorldContextObject)
{
	if (!PlayerActor) return;

	CurrentEnemyParty = InEnemyParty;

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

TArray<FInventorySlotStruct> UBattleManagerSubsystem::GenerateDropItem(UEnemyDataAsset* EnemyData)
{
	TArray<FInventorySlotStruct> ResultItems;

	if (!EnemyData) return ResultItems;

	for (const FEnemyDropInfo& DropInfo : EnemyData->DropTable)
	{
		if (FMath::FRand() > DropInfo.DropChance)
		{
			continue;
		}

		UItemDataAsset* ItemAsset = DropInfo.ItemData.LoadSynchronous();
		if (!ItemAsset) continue;

		EItemGrade SelectedGrade = CalculateDropGrade(DropInfo.MinGrade, DropInfo.MaxGrade);

		FInventorySlotStruct NewItem;
		NewItem.ItemData = ItemAsset;
		NewItem.Grade = SelectedGrade;
		NewItem.Quantity = DropInfo.DropQuantity;

		ResultItems.Add(NewItem);
	}

	return ResultItems;
}

EItemGrade UBattleManagerSubsystem::CalculateDropGrade(EItemGrade MinGrade, EItemGrade MaxGrade)
{
	static const TMap<EItemGrade, float> GradeWeights =
	{
		{ EItemGrade::EIG_S, 2.0f },
		{ EItemGrade::EIG_A, 8.0f },
		{ EItemGrade::EIG_B, 15.0f },
		{ EItemGrade::EIG_C, 25.0f },
		{ EItemGrade::EIG_D, 25.0f },
		{ EItemGrade::EIG_E, 25.0f },
	};

	TArray<EItemGrade> Candidates;
	TArray<float> Weights;
	float TotalWeight = 0.0f;

	int32 MinVal = (int32)MinGrade;
	int32 MaxVal = (int32)MaxGrade;

	if (MinVal > MaxVal) Swap(MinVal, MaxVal);

	for (const auto& Pair : GradeWeights)
	{
		int32 CurrentVal = (int32)Pair.Key;

		if (CurrentVal >= MinVal && CurrentVal <= MaxVal)
		{
			Candidates.Add(Pair.Key);
			Weights.Add(Pair.Value);
			TotalWeight += Pair.Value;
		}
	}

	if (TotalWeight <= 0.0f) return MinGrade;

	float RandomPoint = FMath::FRandRange(0.0f, TotalWeight);

	for (int32 i = 0; i < Candidates.Num(); i++)
	{
		RandomPoint -= Weights[i];
		if (RandomPoint <= 0.0f)
		{
			return Candidates[i];
		}
	}

	return Candidates.Last();
}

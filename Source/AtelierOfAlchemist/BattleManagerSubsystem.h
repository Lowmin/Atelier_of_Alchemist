#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataAssets/EnemyDataAsset.h"
#include "InventoryManagerSubsystem.h"
#include "BattleManagerSubsystem.generated.h"

class UEnemyPartyDataAsset;

UCLASS()
class ATELIEROFALCHEMIST_API UBattleManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void StartBattle(AActor* PlayerActor, AActor* EnemyActor, UEnemyPartyDataAsset* InPartyData, UObject* WorldContextObject);

	void SaveFieldLocation(APawn* PlayerPawn);
	bool GetSavedFieldLocation(FVector& OutLoc, FRotator& OutRot);

	bool bIsBattle = false;

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
	FName LastEncounterSymbolName;

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
	FVector SavedPlayerLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
	FRotator SavedPlayerRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
	FName SavedFieldLevelName;

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
	TObjectPtr<UEnemyPartyDataAsset> CurrentEnemyPartyData;

	UFUNCTION(BlueprintPure, Category = "Battle")
	FName GetBattleLevelName(FName CurrentFieldLevelName) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Battle")
	TMap<FName, FName> FieldToBattleMapName;

	TArray<FInventorySlotStruct> GenerateDropItem(UEnemyDataAsset* EnemyData);

	EItemGrade CalculateDropGrade(EItemGrade MinGrade, EItemGrade MaxGrade);
};

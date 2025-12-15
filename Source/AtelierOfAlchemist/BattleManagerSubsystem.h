// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BattleManagerSubsystem.generated.h"

class UEnemyPartyDataAsset;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UBattleManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void SaveFieldLocation(APawn* PlayerPawn);
	bool GetSavedFieldLocation(FVector& OutLoc, FRotator& OutRot);

	bool IsBattle = false;

	UPROPERTY(BlueprintReadWrite, Category = "Battle")
	FName TargetSymbolName{};

	UPROPERTY(BlueprintReadWrite, Category = "Battle")
	FVector SavedPlayerLocation{};

	UPROPERTY(BlueprintReadWrite, Category = "Battle")
	FRotator SavedPlayerRotation{};

	UPROPERTY(BlueprintReadWrite, Category = "Battle")
	FName SavedFieldLevelName{};

	UFUNCTION(BlueprintPure, Category = "Battle")
	FName GetBattleLevelName(FName CurrentFieldLevelName) const;

	UPROPERTY()
	TSoftObjectPtr<UEnemyPartyDataAsset> EnemyPartyData;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Battle")
	TMap<FName, FName> FieldToBattleMapName;
};

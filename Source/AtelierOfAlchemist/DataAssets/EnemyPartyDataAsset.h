// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyPartyDataAsset.generated.h"

class UCharacterDataAsset;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCharacterDataAsset> EnemyData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SpawnIndex = 0;
};

UCLASS()
class ATELIEROFALCHEMIST_API UEnemyPartyDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Party")
	TArray<FEnemySpawnInfo> EnemyMembers;
};

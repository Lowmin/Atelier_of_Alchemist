// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AoAGameInstance.generated.h"

class UCharacterDataAsset;
class APlayerCharacter;

UCLASS()
class ATELIEROFALCHEMIST_API UAoAGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BattleData")
	TArray<UCharacterDataAsset*> EnemyDatas;
};

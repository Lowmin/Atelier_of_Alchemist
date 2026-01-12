// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FDialogueScenario
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MinLevelCondition = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FText> Dialogues;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName QuestID = NAME_None;
};

USTRUCT(BlueprintType)
struct FNPCData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NPCName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Portrait;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDialogueScenario> Scenarios;
};
#pragma once

#include "CoreMinimal.h"
#include "../../DataAssets/ItemDataAsset.h"

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	Hunting		UMETA(DisplayName = "사냥"),
	Collection	UMETA(DisplayName = "수집")
};

USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQuestType QuestType = EQuestType::Collection;

	// 퀘스트 목표의 ID (사냥이면 MonsterID, 수집이면 ItemID)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TargetCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UItemDataAsset* RewardItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RewardExp;
};
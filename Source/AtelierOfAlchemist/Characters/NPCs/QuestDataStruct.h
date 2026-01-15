#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../../DataAssets/ItemDataAsset.h"
#include "../../DataAssets/CharacterDataAsset.h"
#include "QuestDataStruct.generated.h"

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	Hunting		UMETA(DisplayName = "Hunt"),
	Collection	UMETA(DisplayName = "Collect")
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target", meta = (EditCondition = "QuestType == EQuestType::Collection", EditConditionHides))
	TObjectPtr<UItemDataAsset> TargetItemAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target", meta = (EditCondition = "QuestType == EQuestType::Hunting", EditConditionHides))
	TObjectPtr<UCharacterDataAsset> TargetMonsterAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TargetCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UItemDataAsset* RewardItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemGrade RewardItemGrade;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RewardItemQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RewardExp;
};

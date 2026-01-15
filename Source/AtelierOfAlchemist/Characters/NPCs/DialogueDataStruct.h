#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueDataStruct.generated.h"

UENUM(BlueprintType)
enum class EQuestCondition : uint8
{
	None				UMETA(DisplayName = "조건 없음"),
	NotStarted			UMETA(DisplayName = "퀘스트 시작 전"),
	InProgress			UMETA(DisplayName = "퀘스트 진행 중"),
	Completed			UMETA(DisplayName = "퀘스트 완료"),
};

UENUM(BlueprintType)
enum class EDialogueType : uint8
{
	Normal				UMETA(DisplayName = "일반"),
	Quest				UMETA(DisplayName = "퀘스트")
};

USTRUCT(BlueprintType)
struct FDialogueData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDialogueType DialogueType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Portrait;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FText> Dialogues;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest", meta = (EditCondition = "DialogueType == EDialogueType::Quest", EditConditionHides))
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest", meta = (EditCondition = "DialogueType == EDialogueType::Quest", EditConditionHides))
	EQuestCondition QuestCondition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest", meta = (EditCondition = "DialogueType == EDialogueType::Quest", EditConditionHides))
	int32 ReqLevel;
};

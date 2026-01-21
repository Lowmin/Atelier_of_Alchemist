#pragma once

#include "CoreMinimal.h"
#include "CharacterDataAsset.h"
#include "ItemDataAsset.h"
#include "EnemyDataAsset.generated.h"

UENUM(BlueprintType)
enum class EAIConditionType : uint8
{
	None,
	SelfHP_Under,	// 내 체력이 일정 이하일 때
	AllyHP_Under,	// 아군의 체력이 일정 이하일 때
	Turn_Multiple,	// 일정 턴에서
	Turn_Special,	// 특정 턴에서
	Chance,			// 랜덤 패턴
};

UENUM(BlueprintType)
enum class EAITargetType : uint8
{
	Player_Random,	// 플레이어 중 랜덤
	Player_Weakest,	// 체력이 가장 낮은 플레이어
	Ally_Weakest,	// 체력이 가장 낮은 아군
	Self			// 자신
};

USTRUCT(BlueprintType)
struct FAIPattern
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EAIConditionType ConditionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float ConditionValue = 0.0f;		// 컨디션 수치

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	USkillDataAsset* SkillAsset;		// 사용할 스킬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EAITargetType TargetType;			// 대상

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsOneTimeOnly;				// 한 번만 나오는 패턴
};

USTRUCT(BlueprintType)
struct FEnemyDropInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UItemDataAsset> ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DropChance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemGrade MinGrade = EItemGrade::EIG_E;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemGrade MaxGrade = EItemGrade::EIG_A;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropQuantity;
};

UCLASS()
class ATELIEROFALCHEMIST_API UEnemyDataAsset : public UCharacterDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<FAIPattern> AIPatterns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	TArray<FEnemyDropInfo> DropTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	int32 DropExp = 0;
};

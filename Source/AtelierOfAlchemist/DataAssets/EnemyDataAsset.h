#pragma once

#include "CoreMinimal.h"
#include "CharacterDataAsset.h"
#include "ItemDataAsset.h"
#include "EnemyDataAsset.generated.h"

UENUM(BlueprintType)
enum class EAICondition : uint8
{
	Always,
	HP_LessThan,
	Random_Chance,
	Turn_MultipleOf
};

UENUM(BlueprintType)
enum class EAITargetType : uint8
{
	Random,
	Weakest_HP,
	Highest_HP
};

USTRUCT(BlueprintType)
struct FAIPattern
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAICondition Condition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConditionValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAITargetType TargetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority;
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

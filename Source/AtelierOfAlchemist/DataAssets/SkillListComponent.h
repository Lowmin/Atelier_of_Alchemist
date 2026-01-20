// SkillListComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillListComponent.generated.h"

class USkillDataAsset;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ATELIEROFALCHEMIST_API USkillListComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillListComponent();

	// 외부에서 데이터를 꽂아주는 함수
	void InitializeSkills(const TArray<TObjectPtr<USkillDataAsset>>& InDefaultSkills, int32 CurrentLevel);

	USkillDataAsset* GetSkillIndex(int32 Index) const;

	const TArray<TObjectPtr<USkillDataAsset>>& GetSkillList() const { return SkillLists; }

protected:
	// 실제 런타임에 사용할 스킬 리스트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TArray<TObjectPtr<USkillDataAsset>> SkillLists;
};
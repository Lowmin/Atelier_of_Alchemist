// SkillListComponent.cpp

#include "SkillListComponent.h"
#include "SkillDataAsset.h"

USkillListComponent::USkillListComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillListComponent::InitializeSkills(const TArray<TObjectPtr<USkillDataAsset>>& InDefaultSkills, int32 CurrentLevel)
{
	SkillLists.Empty();

	UE_LOG(LogTemp, Warning, TEXT("[SkillListComponent] 초기화 시작 (Owner: %s) - 원본 개수: %d, 레벨: %d"),
		*GetOwner()->GetName(), InDefaultSkills.Num(), CurrentLevel);

	for (const TObjectPtr<USkillDataAsset>& Skill : InDefaultSkills)
	{
		if (!Skill) continue;

		// 레벨 조건 확인 (0레벨 스킬은 무조건 포함)
		if (Skill->UnlockLevel <= CurrentLevel)
		{
			SkillLists.AddUnique(Skill);
			UE_LOG(LogTemp, Log, TEXT("   + 스킬 등록 성공: %s"), *Skill->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("   - 스킬 레벨 부족: %s (필요: %d)"), *Skill->GetName(), Skill->UnlockLevel);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[SkillListComponent] 초기화 완료 - 최종 등록된 스킬 개수: %d"), SkillLists.Num());
}

USkillDataAsset* USkillListComponent::GetSkillIndex(int32 Index) const
{
	if (SkillLists.IsValidIndex(Index))
	{
		return SkillLists[Index];
	}
	return nullptr;
}
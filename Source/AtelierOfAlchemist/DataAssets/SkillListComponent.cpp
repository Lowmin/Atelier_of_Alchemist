// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillListComponent.h"
#include "SkillDataAsset.h"

USkillListComponent::USkillListComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillListComponent::UpdateSkills(int32 CurrentLevel)
{
	arrSkill.Empty();

	for (USkillDataAsset* Skill : arrDefaultSkill)
	{
		if (Skill->UnlockLevel <= CurrentLevel)
		{
			arrSkill.AddUnique(Skill);
		}
	}
}

USkillDataAsset* USkillListComponent::GetSkillIndex(int32 Index) const
{
	if (arrSkill.IsValidIndex(Index))
	{
		return arrSkill[Index];
	}
	return nullptr;
}

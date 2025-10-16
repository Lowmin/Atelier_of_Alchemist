// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillListComponent.h"
#include "SkillDataAsset.h"

USkillListComponent::USkillListComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillListComponent::BeginPlay()
{
	Super::BeginPlay();

	for (USkillDataAsset* Skill : arrDefaultSkill)
	{
		AddSkill(Skill);
	}
}

void USkillListComponent::AddSkill(USkillDataAsset* Skill)
{
	if (Skill) arrSkill.AddUnique(Skill);
}

void USkillListComponent::UseSkill(int32 SkillIndex)
{
	if (arrSkill.IsValidIndex(SkillIndex) && arrSkill[SkillIndex] != nullptr)
	{
		USkillDataAsset* UseSkillData = arrSkill[SkillIndex];
	}
}

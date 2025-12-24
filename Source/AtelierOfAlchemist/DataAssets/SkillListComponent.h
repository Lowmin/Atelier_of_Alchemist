// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillListComponent.generated.h"

class USkillDataAsset;

UCLASS()
class ATELIEROFALCHEMIST_API USkillListComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	USkillListComponent();

	void UpdateSkills(int32 CurrentLevel);
	USkillDataAsset* GetSkillIndex(int32 Index) const;

	const TArray<USkillDataAsset*>& GetSkillList() const { return arrSkill; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TArray<USkillDataAsset*> arrDefaultSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TArray<USkillDataAsset*> arrSkill;
};

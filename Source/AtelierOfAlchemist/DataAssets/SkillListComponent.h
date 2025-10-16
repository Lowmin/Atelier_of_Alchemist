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

public:	
	void AddSkill(USkillDataAsset* Skill);
	void UseSkill(int32 SkillIndex);

	UFUNCTION(BlueprintPure, Category = "Skill")
	const TArray<USkillDataAsset*>& GetSkillList() const { return arrSkill; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TArray<USkillDataAsset*> arrDefaultSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TArray<USkillDataAsset*> arrSkill;
};

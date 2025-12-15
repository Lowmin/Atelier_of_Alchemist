// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleUnit.h"
#include "Components/WidgetComponent.h"
#include "../../Characters/StatComponent.h"

ABattleUnit::ABattleUnit()
{
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(150.0f, 30.0f));
}

USkillDataAsset* ABattleUnit::GetSkill(int32 Index) const
{
	if (SkillList.IsValidIndex(Index))
	{
		return SkillList[Index];
	}
	return false;
}

void ABattleUnit::TurnStart()
{

}

void ABattleUnit::BattleAction_UseSkill(USkillDataAsset* Skill, const TArray<ABattleUnit*>& Targets)
{
	if (!Skill) return;

	for (ABattleUnit* TargetUnit : Targets)
	{

	}
}

float ABattleUnit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (StatComponent)
	{
		StatComponent->TakeDamage(ActualDamage);

		if (StatComponent->GetCurrentHealth() <= 0)
		{
			
		}
	}

	return ActualDamage;
}


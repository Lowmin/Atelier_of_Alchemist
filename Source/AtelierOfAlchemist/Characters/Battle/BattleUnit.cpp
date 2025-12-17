// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleUnit.h"
#include "Components/WidgetComponent.h"
#include "../../Characters/StatComponent.h"
#include "../../DataAssets/SkillDataAsset.h"

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

	return nullptr;
}

void ABattleUnit::TurnStart()
{

}

void ABattleUnit::BattleAction_UseSkill(USkillDataAsset* Skill, const TArray<ABattleUnit*>& Targets)
{
	if (!Skill) return;

	for (ABattleUnit* TargetUnit : Targets)
	{
		if (Skill->SkillMontage)
		{
			PlayAnimMontage(Skill->SkillMontage);
		}
		else
		{
			if (Skill->SkillType == ESkillType::Melee)
			{
				OnAnimNotify_MeleeHit();
			}
			else
			{
				OnAnimNotify_ShootProjectile();
			}
		}
	}
}

void ABattleUnit::OnAnimNotify_MeleeHit()
{
	for (ABattleUnit* Target : CachedTargets)
	{
		if (Target && Target->GetCurHealth() > 0)
		{
			ApplyDamage(Target, CachedCurrentSkill);
		}
	}
}

void ABattleUnit::OnAnimNotify_ShootProjectile()
{
	// 광역 투사체의 경우
	if (CachedCurrentSkill->IsGlobalProjectile)
	{
		FVector CenterLocation = FVector::ZeroVector;
		int32 Count = 0;

		for (ABattleUnit* Target : CachedTargets)
		{
			if (Target)
			{
				CenterLocation += Target->GetActorLocation();
				Count++;
			}
		}
		if (Count > 0)
		{
			CenterLocation /= Count;
		}
		else
		{
			CenterLocation = GetActorLocation() + GetActorForwardVector() * 500.0f;
		}
	}
	// 
}

void ABattleUnit::ApplySkillEffect(ABattleUnit* Target, USkillDataAsset* Skill)
{

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

void ABattleUnit::ApplyDamage(ABattleUnit* Target, USkillDataAsset* Skill)
{

}

FVector ABattleUnit::ProjectileSpawnPoint(FVector TargetPos)
{
	FVector ResultLoc = GetActorLocation();

	switch (CachedCurrentSkill->ProjectileSpawnType)
	{
	case EProjectileSpawnType::FromCaster:
		ResultLoc = GetActorLocation() + GetActorForwardVector() * 100.0f;
		break;
	case EProjectileSpawnType::FromSky:
		ResultLoc = TargetPos + FVector(0.0f, 0.0f, 1000.0f);
		break;
	case EProjectileSpawnType::AtLocation:
		ResultLoc = TargetPos;
		break;
	default:
		break;
	}

	return ResultLoc;
}


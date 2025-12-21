#include "BattleUnit.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleUnit.h"
#include "Components/WidgetComponent.h"
#include "../../Characters/StatComponent.h"
#include "../../DataAssets/SkillDataAsset.h"
#include "../../Object/BattleProjectile.h"
#include "../../BattleGameMode.h"

ABattleUnit::ABattleUnit()
{
	TargetMarkerWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetMarkerWidget"));
	TargetMarkerWidget->SetupAttachment(RootComponent);
	TargetMarkerWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TargetMarkerWidget->SetDrawSize(FVector2D(50.0f, 50.0f));
	TargetMarkerWidget->SetVisibility(false);
	TargetMarkerWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
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

	UE_LOG(LogTemp, Warning, TEXT("[BattleUnit] Skill Action Started!"));

	if (Skill && Skill->SkillMontage)
	{
		float Duration = PlayAnimMontage(Skill->SkillMontage);

		float WaitTime = (Duration > 0.0f) ? Duration : 1.0f;

		FTimerHandle TurnEndHandle;
		GetWorld()->GetTimerManager().SetTimer(TurnEndHandle, this, &ABattleUnit::NotifyTurnEnd, WaitTime, false);
	}
	else
	{
		FTimerHandle TurnEndHandle;
		GetWorld()->GetTimerManager().SetTimer(TurnEndHandle, this, &ABattleUnit::NotifyTurnEnd, 1.0f, false);
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

		// 광역 타겟의 중간 Location을 구함
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

		FVector SpawnLoc = ProjectileSpawnPoint(CenterLocation);
		FRotator SpawnRot = ProjectileSpawnRotation(CenterLocation, SpawnLoc);

		ABattleProjectile* Projectile = GetWorld()->SpawnActor<ABattleProjectile>
			(CachedCurrentSkill->ProjectileClass, SpawnLoc, SpawnRot);

		if (Projectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("광역 투사체 생성."));
			Projectile->InitializeGlobal(CachedTargets, CachedCurrentSkill, this, CenterLocation);
		}
	}
	// 단일 투사체의 경우
	else
	{
		for (ABattleUnit* Target : CachedTargets)
		{
			if (Target)
			{
				FVector TargetPos = Target->GetActorLocation();

				FVector SpawnLoc = ProjectileSpawnPoint(TargetPos);
				FRotator SpawnRot = ProjectileSpawnRotation(TargetPos, SpawnLoc);

				ABattleProjectile* Projectile = GetWorld()->SpawnActor<ABattleProjectile>
					(CachedCurrentSkill->ProjectileClass, SpawnLoc, SpawnRot);

				if (Projectile)
				{
					UE_LOG(LogTemp, Warning, TEXT("단일 투사체 생성."));
					Projectile->InitializeSingle(Target, CachedCurrentSkill, this);
				}
			}
		}
	}
}

void ABattleUnit::NotifyTurnEnd()
{
	if (ABattleGameMode* GM = GetWorld()->GetAuthGameMode<ABattleGameMode>())
	{
		GM->TurnEnd();
	}
}

void ABattleUnit::ApplySkillEffect(ABattleUnit* Target, USkillDataAsset* Skill)
{

}

void ABattleUnit::SetTargetSelect(bool IsSelected)
{
	if (TargetMarkerWidget)
	{
		TargetMarkerWidget->SetVisibility(IsSelected);
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

FRotator ABattleUnit::ProjectileSpawnRotation(FVector TargetPos, FVector SpawnLocation)
{
	FRotator ResultRot = FRotator::ZeroRotator;

	switch (CachedCurrentSkill->ProjectileSpawnType)
	{
	case EProjectileSpawnType::FromCaster:
		ResultRot = (TargetPos - SpawnLocation).Rotation();
		break;
	case EProjectileSpawnType::FromSky:
		ResultRot = FRotator(-90.0, 0.0f, 0.0f);
		break;
	case EProjectileSpawnType::AtLocation:
		ResultRot = FRotator::ZeroRotator;
		break;
	default:
		break;
	}

	return FRotator();
}


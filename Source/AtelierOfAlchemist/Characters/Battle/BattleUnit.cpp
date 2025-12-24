#include "BattleUnit.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../Characters/StatComponent.h"
#include "../../DataAssets/SkillDataAsset.h"
#include "../../DataAssets/CharacterDataAsset.h"
#include "../../Object/BattleProjectile.h"
#include "../../BattleGameMode.h"

ABattleUnit::ABattleUnit()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TargetMarkerWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetMarkerWidget"));
	TargetMarkerWidget->SetupAttachment(RootComponent);
	TargetMarkerWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TargetMarkerWidget->SetDrawSize(FVector2D(50.0f, 50.0f));
	TargetMarkerWidget->SetVisibility(false);
	TargetMarkerWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	if (GetMesh())
	{
		WeaponMesh->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	}
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void ABattleUnit::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		AIController = GetWorld()->SpawnActor<AAIController>(FVector::ZeroVector, FRotator::ZeroRotator);
		if (AIController)
		{
			AIController->Possess(this);
		}
	}

	PreloadSkillAssets();
}

void ABattleUnit::PreloadSkillAssets()
{
	PreloadAssets.Empty();

	for (USkillDataAsset* Skill : SkillList)
	{
		if (!Skill) continue;

		if (!Skill->SkillMontage.IsNull())
		{
			if (UAnimMontage* LoadedMontage = Skill->SkillMontage.LoadSynchronous())
			{
				PreloadAssets.Add(LoadedMontage);
			}
		}

		if (Skill->SkillType == ESkillType::Projectile && !Skill->ProjectileClass.IsNull())
		{
			if (UClass* LoadedClass = Skill->ProjectileClass.LoadSynchronous())
			{
				PreloadAssets.Add(LoadedClass);
			}
		}
	}
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
	if (!Skill || Targets.IsEmpty())
	{
		NotifyTurnEnd();
		return;
	}

	CachedCurrentSkill = Skill;
	CachedTargets = Targets;
	OriginalLocation = GetActorLocation();
	bIsReturning = false;

	if (Skill->SkillType == ESkillType::Melee && AIController)
	{
		if (ABattleUnit* Target = Targets[0])
		{
			FVector TargetPos = Target->GetActorLocation();
			FVector Direction = (GetActorLocation() - TargetPos).GetSafeNormal();

			FVector DestPos = TargetPos + (Direction * 100.0f);

			FAIMoveRequest MoveReq;
			MoveReq.SetGoalLocation(DestPos);
			MoveReq.SetAcceptanceRadius(10.0f);

			AIController->ReceiveMoveCompleted.AddDynamic(this, &ABattleUnit::OnMoveCompleted);
			AIController->MoveTo(MoveReq);
		}
		else
		{
			StartAttackSequence();
		}
	}
	else
	{
		if (Targets[0])
		{
			FVector LookDir = Targets[0]->GetActorLocation() - GetActorLocation();
			LookDir.Z = 0.0f;
			SetActorRotation(LookDir.Rotation());
		}
		StartAttackSequence();
	}
}

void ABattleUnit::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (AIController)
	{
		AIController->ReceiveMoveCompleted.RemoveDynamic(this, &ABattleUnit::OnMoveCompleted);
	}

	if (bIsReturning)
	{
		if (CachedTargets.IsValidIndex(0) && CachedTargets[0])
		{
			FVector LookDir = CachedTargets[0]->GetActorLocation() - GetActorLocation();
			LookDir.Z = 0.0f;
			SetActorRotation(LookDir.Rotation());
		}
		NotifyTurnEnd();
	}
	else
	{
		StartAttackSequence();
	}
}

void ABattleUnit::StartAttackSequence()
{
	if (!CachedCurrentSkill) return;

	float Duration = 0.0f;

	UAnimMontage* MontageToPlay = CachedCurrentSkill->SkillMontage.LoadSynchronous();

	if (MontageToPlay)
	{
		Duration = PlayAnimMontage(MontageToPlay);
	}
	else
	{
		if (CachedCurrentSkill->SkillType == ESkillType::Melee)
			OnAnimNotify_MeleeHit();
		else
			OnAnimNotify_ShootProjectile();

		Duration = 1.0f;
	}

	float WaitTime = (Duration > 0.0f) ? Duration : 1.0f;

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ABattleUnit::OnAttackSequenceFinished, WaitTime, false);
}

void ABattleUnit::OnAttackSequenceFinished()
{
	if (CachedCurrentSkill && CachedCurrentSkill->SkillType == ESkillType::Melee)
	{
		bIsReturning = true;

		if (AIController)
		{
			FAIMoveRequest MoveReq;
			MoveReq.SetGoalLocation(OriginalLocation);
			MoveReq.SetAcceptanceRadius(5.0f);

			AIController->ReceiveMoveCompleted.AddDynamic(this, &ABattleUnit::OnMoveCompleted);
			AIController->MoveTo(MoveReq);
		}
		else
		{
			NotifyTurnEnd();
		}
	}
	else
	{
		NotifyTurnEnd();
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
	if (!CachedCurrentSkill) return;

	if (CachedTargets.IsValidIndex(0) && CachedTargets[0])
	{
		UClass* ProjectileClass = CachedCurrentSkill->ProjectileClass.LoadSynchronous();

		if (ProjectileClass)
		{
			FVector TargetPos = CachedTargets[0]->GetActorLocation();

			FVector SpawnLoc = ProjectileSpawnPoint(TargetPos);
			FRotator SpawnRot = ProjectileSpawnRotation(TargetPos, SpawnLoc);

			FActorSpawnParameters Params;
			Params.Owner = this;
			Params.Instigator = this;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ABattleProjectile* Projectile = GetWorld()->SpawnActor<ABattleProjectile>(ProjectileClass, SpawnLoc, SpawnRot, Params);

			if (Projectile)
			{
				//Projectile->InitializeProjectile(CachedTargets[0], CachedCurrentSkill);
			}
		}
	}
}

void ABattleUnit::ApplyDamage(ABattleUnit* Target, USkillDataAsset* Skill)
{
	if (!Target || !Skill) return;

	UGameplayStatics::ApplyDamage(Target, Skill->Power, GetController(), this, nullptr);
}

float ABattleUnit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (StatComponent)
	{
		StatComponent->TakeDamage(ActualDamage);
	}

	return ActualDamage;
}

void ABattleUnit::NotifyTurnEnd()
{
	bIsReturning = false;

	if (ABattleGameMode* GM = GetWorld()->GetAuthGameMode<ABattleGameMode>())
	{
		GM->TurnEnd();
	}
}

void ABattleUnit::SetTargetSelect(bool IsSelected)
{
	if (TargetMarkerWidget)
	{
		TargetMarkerWidget->SetVisibility(IsSelected);
	}
}

void ABattleUnit::ApplySkillEffect(ABattleUnit* Target, USkillDataAsset* Skill)
{

}

FVector ABattleUnit::ProjectileSpawnPoint(FVector TargetPos)
{
	FVector ResultLoc = GetActorLocation();

	if (!CachedCurrentSkill) return ResultLoc;

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
	if (!CachedCurrentSkill) return FRotator::ZeroRotator;

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
	return ResultRot;
}
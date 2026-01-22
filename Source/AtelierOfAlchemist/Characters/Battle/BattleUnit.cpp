#include "BattleUnit.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "../../DataAssets/CharacterDataAsset.h"
#include "../../DataAssets/SkillListComponent.h"
#include "../../DataAssets/SkillDataAsset.h"
#include "../../DataAssets/EnemyDataAsset.h"
#include "../../Characters/StatComponent.h"
#include "../../Object/BattleProjectile.h"
#include "../Enemy/BattleAIComponent.h"
#include "../../BattleGameMode.h"

ABattleUnit::ABattleUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	SkillComponent = CreateDefaultSubobject<USkillListComponent>("SkillComponent");
	BattleAIComponent = CreateDefaultSubobject<UBattleAIComponent>("BattleAIComponent");

	TargetMarkerWidget = CreateDefaultSubobject<UWidgetComponent>("TargetMarkerWidget");
	TargetMarkerWidget->SetupAttachment(RootComponent);
	TargetMarkerWidget->SetVisibility(false);
	TargetMarkerWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	TargetMarkerWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TargetMarkerWidget->SetDrawSize(FVector2D(50.0f, 50.0f));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	PendingDamage = 0.0f;
	ActionState = EUnitActionState::Idle;
}

void ABattleUnit::InitializeAsPlayerUnit(UPlayerRuntimeData* RuntimeData)
{
	Type = ECharacterType::Player;

	if (!RuntimeData || !StatComponent || !SkillComponent) return;

	UCharacterDataAsset* DataAsset = RuntimeData->GetCharacterDataAsset();

	StatComponent->Initialize(RuntimeData);
	SkillComponent->InitializeSkills(DataAsset->DefaultSkills, RuntimeData->GetLevel());

	PreloadAssetsFromSkills();
}

void ABattleUnit::InitializeAsEnemyUnit(UEnemyDataAsset* DataAsset, int32 Level)
{
	Type = ECharacterType::Enemy;

	if (!DataAsset || !StatComponent || !SkillComponent) return;

	StatComponent->InitializeFromEnemy(DataAsset);
	SkillComponent->InitializeSkills(DataAsset->DefaultSkills, Level);

	if (BattleAIComponent)
	{
		BattleAIComponent->AIPatterns = DataAsset->AIPatterns;
	}

	PreloadAssetsFromSkills();
}

void ABattleUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActionState == EUnitActionState::MoveToTarget)
	{
		FVector CurrentLoc = GetActorLocation();
		FVector NewLoc = FMath::VInterpConstantTo(CurrentLoc, MoveDestination, DeltaTime, 600.0f);

		SetActorLocation(NewLoc);

		FVector Direction = (MoveDestination - CurrentLoc).GetSafeNormal();
		if (!Direction.IsNearlyZero())
		{
			FRotator TargetRot = FRotationMatrix::MakeFromX(Direction).Rotator();
			TargetRot.Pitch = 0.0f;
			TargetRot.Roll = 0.0f;
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 15.0f));
		}

		if (NewLoc.Equals(MoveDestination, 1.0f))
		{
			ActionState = EUnitActionState::Attacking;
			PlayAttackMontage();
		}
	}
	else if (ActionState == EUnitActionState::ReturnToPos)
	{
		FVector CurrentLoc = GetActorLocation();
		FVector NewLoc = FMath::VInterpConstantTo(CurrentLoc, OriginalLocation, DeltaTime, 600.0f);

		SetActorLocation(NewLoc);

		FVector Direction = (OriginalLocation - CurrentLoc).GetSafeNormal();
		if (!Direction.IsNearlyZero())
		{
			FRotator TargetRot = FRotationMatrix::MakeFromX(Direction).Rotator();
			TargetRot.Pitch = 0.0f;
			TargetRot.Roll = 0.0f;
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 15.0f));
		}

		if (NewLoc.Equals(OriginalLocation, 1.0f))
		{
			SetActorLocation(OriginalLocation);
			SetActorRotation(OriginalRotation);

			ActionState = EUnitActionState::Idle;

			ABattleGameMode* BattleGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());
			if (BattleGameMode)
			{
				BattleGameMode->ProcessNextTurn();
			}
		}
	}
}

void ABattleUnit::SetTargetSelected(bool bIsSelected)
{
	if (TargetMarkerWidget)
	{
		TargetMarkerWidget->SetVisibility(bIsSelected);
	}
}

void ABattleUnit::StartAttack(ABattleUnit* Target, float FinalDamage, USkillDataAsset* SkillAsset)
{
	if (!Target || !SkillAsset) return;

	PendingTarget = Target;
	PendingDamage = FinalDamage;
	CurrentSkill = SkillAsset;
	OriginalLocation = GetActorLocation();
	OriginalRotation = GetActorRotation();

	if (SkillAsset->SkillType == ESkillType::Melee)
	{
		FVector TargetLoc = Target->GetActorLocation();
		TargetLoc.Z = GetActorLocation().Z;

		FVector Direction = (TargetLoc - GetActorLocation()).GetSafeNormal();

		float MyRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
		float TargetRadius = Target->GetCapsuleComponent()->GetScaledCapsuleRadius();
		float StopDist = MyRadius + TargetRadius + 50.0f;

		MoveDestination = TargetLoc - (Direction * StopDist);

		ActionState = EUnitActionState::MoveToTarget;
	}
	else
	{
		ActionState = EUnitActionState::Attacking;
		PlayAttackMontage();
	}
}

void ABattleUnit::OnAttackHit()
{
	if (PendingTarget && PendingTarget->IsValidLowLevel())
	{
		PendingTarget->TakeDamage(PendingDamage, FDamageEvent(), GetController(), this);
		UE_LOG(LogTemp, Warning, TEXT("Attack! %f Damage"), PendingDamage);
	}
}

void ABattleUnit::OnDodgeBegin()
{
	bIsDodge = true;
}

void ABattleUnit::OnDodgeEnd()
{
	bIsDodge = false;
}

void ABattleUnit::OnAttackAnimationEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (CurrentSkill && CurrentSkill->SkillType == ESkillType::Melee)
	{
		ActionState = EUnitActionState::ReturnToPos;
	}
	else
	{
		ActionState = EUnitActionState::Idle;
		ABattleGameMode* BattleGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());
		if (BattleGameMode)
		{
			BattleGameMode->ProcessNextTurn();
		}
	}
}

float ABattleUnit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!StatComponent) return 0.0f;
	if (bIsDodge) return 0.0f;

	float ActualDamage = StatComponent->ApplyDamage(DamageAmount);
	
	UAnimMontage* HitMontage = CachedCharacterDataAsset->HitMontage.LoadSynchronous();
	if (!HitMontage) return ActualDamage;
	PlayAnimMontage(HitMontage);

	UE_LOG(LogTemp, Warning, TEXT("[%s] Took %.1f Damage! Left HP: %.1f"), *GetName(), ActualDamage, StatComponent->GetCurrentHealth());

	if (StatComponent->GetCurrentHealth() <= 0)
	{
		Die();
	}

	return ActualDamage;
}

void ABattleUnit::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Die: %s"), *GetName());

	ActionState = EUnitActionState::Die;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ABattleGameMode* BattleGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());
}

void ABattleUnit::Dodge()
{
	UAnimMontage* DodgeMontage = CachedCharacterDataAsset->DodgeMontage.LoadSynchronous();
	if (!DodgeMontage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	OnDodgeBegin();

	FOnMontageEnded EndDelegate;
	EndDelegate.BindWeakLambda(this, [this](UAnimMontage* Montage, bool bInterrupted)
		{
			this->OnDodgeEnd();
		});
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeMontage);
}

void ABattleUnit::PlayAttackMontage()
{
	if (!CurrentSkill || CurrentSkill->SkillMontage.IsNull())
	{
		OnAttackHit();
		OnAttackAnimationEnd(nullptr, false);
		return;
	}

	UAnimMontage* Montage = CurrentSkill->SkillMontage.LoadSynchronous();
	if (Montage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(Montage);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &ABattleUnit::OnAttackAnimationEnd);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);
			return;
		}
	}

	OnAttackHit();
	OnAttackAnimationEnd(nullptr, false);
}

void ABattleUnit::PreloadAssetsFromSkills()
{
	if (!SkillComponent) return;

	PreloadedAssets.Empty();

	const TArray<TObjectPtr<USkillDataAsset>>& MySkills = SkillComponent->GetSkillList();

	for (const auto& Skill : MySkills)
	{
		if (!Skill) continue;

		if (!Skill->SkillMontage.IsNull())
		{
			PreloadedAssets.Add(Skill->SkillMontage.LoadSynchronous());
		}

		if (!Skill->ProjectileClass.IsNull())
		{
			PreloadedAssets.Add(Skill->ProjectileClass.LoadSynchronous());
		}
	}
}
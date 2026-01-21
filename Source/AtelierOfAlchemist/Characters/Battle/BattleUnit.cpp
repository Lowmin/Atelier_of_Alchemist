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
		if (!PendingTarget)
		{
			ActionState = EUnitActionState::Idle;
			return;
		}

		FVector TargetLoc = PendingTarget->GetActorLocation();
		FVector Direction = (TargetLoc - GetActorLocation()).GetSafeNormal();

		TargetLoc.Z = GetActorLocation().Z;
		FVector StopLocation = TargetLoc - (Direction * 100.0f);

		FVector NewLoc = FMath::VInterpTo(GetActorLocation(), StopLocation, DeltaTime, 15.0f);
		SetActorLocation(NewLoc);

		FRotator TargetRot = FRotationMatrix::MakeFromX(Direction).Rotator();
		TargetRot.Pitch = 0.0f;
		TargetRot.Roll = 0.0f;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 20.0f));

		if (FVector::Dist(GetActorLocation(), StopLocation) < 10.0f)
		{
			ActionState = EUnitActionState::Attacking;
			PlayAttackMontage();
		}
	}
	else if (ActionState == EUnitActionState::ReturnToPos)
	{
		FVector TargetLoc = OriginalLocation;
		FVector Direction = (TargetLoc - GetActorLocation()).GetSafeNormal();

		FVector NewLoc = FMath::VInterpTo(GetActorLocation(), TargetLoc, DeltaTime, 15.0f);
		SetActorLocation(NewLoc);

		FRotator TargetRot = FRotationMatrix::MakeFromX(Direction).Rotator();
		TargetRot.Pitch = 0.0f;
		TargetRot.Roll = 0.0f;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 20.0f));

		if (FVector::Dist(GetActorLocation(), TargetLoc) < 5.0f)
		{
			SetActorLocation(OriginalLocation);
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

	if (SkillAsset->SkillType == ESkillType::Melee)
	{
		ActionState = EUnitActionState::MoveToTarget;
	}
	else
	{
		ActionState = EUnitActionState::Attacking;
		PlayAttackMontage();
	}
}

void ABattleUnit::PlayAttackMontage()
{
	if (CurrentSkill && !CurrentSkill->SkillMontage.IsNull())
	{
		UAnimMontage* Montage = CurrentSkill->SkillMontage.LoadSynchronous();
		if (Montage)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				PlayAnimMontage(Montage);

				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &ABattleUnit::OnAttackAnimationEnd);
				AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);
				return;
			}
		}
	}

	OnAttackHit();
	OnAttackAnimationEnd(nullptr, false);
}

void ABattleUnit::OnAttackHit()
{
	if (PendingTarget && PendingTarget->IsValidLowLevel())
	{
		PendingTarget->TakeDamage(PendingDamage, FDamageEvent(), GetController(), this);
	}
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

	float ActualDamage = StatComponent->ApplyDamage(DamageAmount);

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
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
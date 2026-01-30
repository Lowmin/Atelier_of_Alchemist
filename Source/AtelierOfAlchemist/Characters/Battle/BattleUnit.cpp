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

	DefaultLocation = GetActorLocation();
	CombatState = EUnitCombatState::Idle;
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

	switch (CombatState)
	{
	case EUnitCombatState::Idle:
		break;
	case EUnitCombatState::Approach:
	{
		FVector CurrentLoc = GetActorLocation();
		// VInterpTo에 비해 이동속도가 일정하므로 턴제 게임의 템포를 살리기 좋아 채택
		FVector NewLoc = FMath::VInterpTo(CurrentLoc, TargetLoc, DeltaTime, 10.0f);

		SetActorLocation(NewLoc);

		FVector Direction = (TargetLoc - CurrentLoc).GetSafeNormal();
		if (!Direction.IsNearlyZero())
		{
			FRotator TargetRot = FRotationMatrix::MakeFromX(Direction).Rotator();
			TargetRot.Pitch = 0.0f;
			TargetRot.Yaw = 0.0f;
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 15.0f));
		}

		if (NewLoc.Equals(TargetLoc, 1.0f))
		{
			SetCombatState(EUnitCombatState::Attacking);
		}
		break;
	}
	case EUnitCombatState::Attacking:
		break;
	case EUnitCombatState::ReturnToPos:
	{
		FVector CurrentLoc = GetActorLocation();
		FVector NewLoc = FMath::VInterpTo(CurrentLoc, DefaultLoc, DeltaTime, 600.0f);

		SetActorLocation(NewLoc);

		FVector Direction = (DefaultLoc - CurrentLoc).GetSafeNormal();
		if (!Direction.IsNearlyZero())
		{
			FRotator TargetRot = FRotationMatrix::MakeFromX(Direction).Rotator();
			TargetRot.Pitch = 0.0f;
			TargetRot.Roll = 0.0f;
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 15.0f));
		}

		// 이동 오차범위
		if (NewLoc.Equals(DefaultLoc, 1.0f))
		{
			SetActorLocation(DefaultLoc);
			SetActorRotation(DefaultRot);

			CombatState = EUnitCombatState::Idle;

			ABattleGameMode* BattleGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());
			if (BattleGameMode)
			{
				BattleGameMode->ProcessNextTurn();
			}
		}
		break;
	}
	}
}

void ABattleUnit::SetCombatState(EUnitCombatState NewCombatState)
{
	CombatState = NewCombatState;
}

float ABattleUnit::CalculateDamage(ABattleUnit* Target, USkillDataAsset* Skill)
{
	float MyAtk = StatComponent->GetAttackPower();
	float TargetDef = Target->GetStatComponent()->GetDefense();
	float SkillRatio = Skill->Power;
	float FinalDamage = (MyAtk * SkillRatio) - TargetDef;

	// 방어력이 최종 데미지보다 높아도 데미지 1 보장
	return FMath::Max(FinalDamage, 1.0f);
}

bool ABattleUnit::MoveToTarget(FVector TargetLoc, float DeltaTime)
{
	FVector CurrentLoc = GetActorLocation();
	FVector NewLoc = FMath::VInterpConstantTo(CurrentLoc, TargetLoc, DeltaTime, MoveSpeed);
	SetActorLocation(NewLoc);

	FVector Direction = (TargetLoc - CurrentLoc).GetSafeNormal();
	if (!Direction.IsNearlyZero())
	{
		FRotator TargetRot = FRotationMatrix::MakeFromX(Direction).Rotator();
		TargetRot.Pitch = 0.0f;
		TargetRot.Yaw = 0.0f;
		FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 15.0f);

		SetActorRotation(NewRot);
	}

	return NewLoc.Equals(TargetLoc, 10.0f);
}

void ABattleUnit::AnimNotify_OnHit()
{
	
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleProjectile.h"
#include "Kismet/GameplayStatics.h"

// 컴포넌트
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "../Characters/Battle/BattleUnit.h"
#include "../DataAssets/SkillDataAsset.h"

// Sets default values
ABattleProjectile::ABattleProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// 충돌
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionProfileName(TEXT("Trigger"));
	SphereComponent->SetSphereRadius(30.0f);

	// 외형
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	// 투사체 이동
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 1500.0f;
	MovementComponent->MaxSpeed = 3000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bIsHomingProjectile = false;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void ABattleProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABattleProjectile::OnOverlap);
}

void ABattleProjectile::InitializeSingle(ABattleUnit* Target, USkillDataAsset* SkillData, ABattleUnit* InInstigator)
{
	SingleTarget = Target;
	SkillDataAsset = SkillData;
	CastUnit = InInstigator;
	IsGlobal = false;

	if (SingleTarget)
	{
		MovementComponent->bIsHomingProjectile = true;
		MovementComponent->HomingTargetComponent = SingleTarget->GetRootComponent();
		MovementComponent->HomingAccelerationMagnitude = 50000.0f;
	}
}

void ABattleProjectile::InitializeGlobal(const TArray<ABattleUnit*>& TargetList, USkillDataAsset* SkillData, ABattleUnit* InInstigator, FVector TargetLocation)
{
	GlobalTargetList = TargetList;
	SkillDataAsset = SkillData;
	CastUnit = InInstigator;
	IsGlobal = true;

	MovementComponent->bIsHomingProjectile = false;

	FVector SpawnLoc = GetActorLocation();
	FVector Direction = (TargetLocation - SpawnLoc).GetSafeNormal();

	MovementComponent->Velocity = Direction * MovementComponent->InitialSpeed;
}

void ABattleProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CastUnit || !SkillDataAsset)
	{
		Destroy();
		return;
	}

	if (OtherActor == CastUnit) return;

	bool IsHit = false;

	if (IsGlobal)
	{
		IsHit = true;

		for (ABattleUnit* Target : GlobalTargetList)
		{
			if (Target)
			{
				CastUnit->ApplySkillEffect(Target, SkillDataAsset);
			}
		}
	}
	else
	{
		if (OtherActor == SingleTarget)
		{
			IsHit = true;
			CastUnit->ApplySkillEffect(SingleTarget, SkillDataAsset);
		}
	}

	if (IsHit)
	{
		// TODO: 여기에 폭발 이펙트(Particle/Niagara) 생성 코드 추가
		// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionVFX, GetActorLocation());

		// TODO: 폭발 사운드 재생
		// UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

		Destroy();
	}
}


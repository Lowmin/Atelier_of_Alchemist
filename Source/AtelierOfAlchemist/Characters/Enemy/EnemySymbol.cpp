// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySymbol.h"
#include "../Playable/PlayerCharacter.h"
#include "../../BattleManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

AEnemySymbol::AEnemySymbol()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	// RootComponent가 없으면 Mesh를 Root로 설정하는 것이 안전합니다.
	RootComponent = Mesh;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(Mesh); // Mesh에 붙임
	DetectionSphere->SetSphereRadius(100.0f);
	DetectionSphere->SetHiddenInGame(false);
	DetectionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // 프로필 이름 확인 필요 (Sensor가 없으면 기본값 추천)
}

void AEnemySymbol::BeginPlay()
{
	Super::BeginPlay();

	if (DetectionSphere)
	{
		// 델리게이트가 중복 등록되지 않게 확인하거나 BeginPlay에서 등록
		DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemySymbol::OnOverlapBegin);
	}
}

void AEnemySymbol::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어인지 확인
	if (OtherActor && Cast<APlayerCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Detected : Starting Battle..."));

		UGameInstance* GI = GetGameInstance();
		if (!GI) return;

		UBattleManagerSubsystem* BattleManager = GI->GetSubsystem<UBattleManagerSubsystem>();

		// [수정] EnemyParty 배열이 비어있지 않은지 확인
		if (BattleManager && EnemyParty.Num() > 0)
		{
			// [수정] 배열(EnemyParty)을 그대로 넘김
			BattleManager->StartBattle(OtherActor, this, EnemyParty, this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("EnemyParty is Empty! Check the Detail Panel of EnemySymbol."));
		}
	}
}
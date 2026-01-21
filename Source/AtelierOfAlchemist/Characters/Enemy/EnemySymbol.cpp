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
	
	RootComponent = Mesh;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(Mesh);
	DetectionSphere->SetSphereRadius(100.0f);
	DetectionSphere->SetHiddenInGame(false);
	DetectionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic")); 
}

void AEnemySymbol::BeginPlay()
{
	Super::BeginPlay();

	if (DetectionSphere)
	{
		DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemySymbol::OnOverlapBegin);
	}
}

void AEnemySymbol::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Cast<APlayerCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Detected : Starting Battle..."));

		UGameInstance* GI = GetGameInstance();
		if (!GI) return;

		UBattleManagerSubsystem* BattleManager = GI->GetSubsystem<UBattleManagerSubsystem>();

		if (BattleManager && EnemyParty.Num() > 0)
		{
			BattleManager->StartBattle(OtherActor, this, EnemyParty, this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("EnemyParty is Empty! Check the Detail Panel of EnemySymbol."));
		}
	}
}
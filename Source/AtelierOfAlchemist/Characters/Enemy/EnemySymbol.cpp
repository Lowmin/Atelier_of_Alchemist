// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySymbol.h"
#include "../Playable/PlayerCharacter.h"
#include "../../BattleManagerSubsystem.h"
#include "../../AoAGameInstance.h"
#include "../../DataAssets/EnemyPartyDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

AEnemySymbol::AEnemySymbol()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(Mesh);
	DetectionSphere->SetSphereRadius(100.0f);
	DetectionSphere->SetHiddenInGame(false);
	DetectionSphere->SetCollisionProfileName(TEXT("Sensor"));
}

void AEnemySymbol::BeginPlay()
{
	Super::BeginPlay();
	
	if (DetectionSphere)
	{
		UE_LOG(LogTemp, Warning, TEXT("DetectionSphere Init."));
		DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemySymbol::OnOverlapBegin);
	}
}

void AEnemySymbol::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Cast<APlayerCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Detected."))
		UBattleManagerSubsystem* BattleManager = GetGameInstance()->GetSubsystem<UBattleManagerSubsystem>();

		if (BattleManager && EnemyPartyData)
		{
			BattleManager->StartBattle(OtherActor, this, EnemyPartyData, this);
		}
	}
}

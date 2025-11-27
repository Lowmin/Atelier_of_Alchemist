// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySymbol.h"
#include "../Playable/PlayerCharacter.h"
#include "../../BattleManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AEnemySymbol::AEnemySymbol()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Mesh->SetupAttachment(RootComponent);

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(100.0f);
	DetectionSphere->SetCollisionProfileName(TEXT("Sensor"));
}

// Called when the game starts or when spawned
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
	if (OtherActor && OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		UBattleManagerSubsystem* BattleManager = GetGameInstance()->GetSubsystem<UBattleManagerSubsystem>();
		
		if (BattleManager)
		{
			FString CurrentLevelString = UGameplayStatics::GetCurrentLevelName(this);
			FName CurrentLevelName = FName(*CurrentLevelString);

			BattleManager->SavedPlayerLocation = OtherActor->GetActorLocation();
			BattleManager->SavedPlayerRotation = OtherActor->GetActorRotation();
			BattleManager->SavedFieldLevelName = CurrentLevelName;

			FName BattleLevelName = BattleManager->GetBattleLevelName(CurrentLevelName);

			UGameplayStatics::OpenLevel(this, BattleLevelName);

			UE_LOG(LogTemp, Warning, TEXT("Encounter! Field: %s -> Battle: %s"), *CurrentLevelName.ToString(), *BattleLevelName.ToString());
		}
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActorBase.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

AInteractableActorBase::AInteractableActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
	TObjectPtr<USceneComponent> Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ObjectSensor = CreateDefaultSubobject<USphereComponent>(TEXT("ObjectSensor"));
	ObjectSensor->SetupAttachment(Mesh);
	ObjectSensor->SetSphereRadius(600.0f);
}

void AInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();

	ObjectSensor->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActorBase::OnBeginOverlap);
	ObjectSensor->OnComponentEndOverlap.AddDynamic(this, &AInteractableActorBase::OnEndOverlap);
}

void AInteractableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableActorBase::OnPlayerEnter_Implementation(APlayerCharacter* PlayerCharacter)
{

}

void AInteractableActorBase::OnPlayerLeave_Implementation(APlayerCharacter* PlayerCharacter)
{

}

void AInteractableActorBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		OnPlayerEnter(PlayerCharacter);
	}
}

void AInteractableActorBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		OnPlayerLeave(PlayerCharacter);
	}
}


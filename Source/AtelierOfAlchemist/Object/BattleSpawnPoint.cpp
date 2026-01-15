// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleSpawnPoint.h"
#include "Components/ArrowComponent.h"

// Sets default values
ABattleSpawnPoint::ABattleSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABattleSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABattleSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


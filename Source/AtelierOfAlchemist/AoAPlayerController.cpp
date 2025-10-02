// Fill out your copyright notice in the Description page of Project Settings.


#include "AoAPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/Playable/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"

AAoAPlayerController::AAoAPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAoAPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void AAoAPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAoAPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "AoAPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/Playable/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/Inventory/Inventory.h"
#include "UI/MainUI.h"
#include "UI/MyHUD.h"
#include "Animation/WidgetAnimation.h"

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

    if (UEnhancedInputComponent* EnhancedInputComonent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComonent->BindAction(IA_ToggleInventory, ETriggerEvent::Started, this, &AAoAPlayerController::ToggleInventory);
    }
}

void AAoAPlayerController::ToggleInventory()
{
}
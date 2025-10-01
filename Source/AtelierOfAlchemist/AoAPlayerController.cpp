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

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(IMC_Default, 0);
    }
}

void AAoAPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Look
    const FRotator CurrentRotation = GetControlRotation();
    const FRotator TargetRotation = FRotator(TargetCameraPitch, TargetCameraYaw, 0.0f);

    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, CameraInterpSpeed);
    SetControlRotation(NewRotation);

    // Zoom
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter)
    {
        if (USpringArmComponent* SpringArm = PlayerCharacter->GetSpringArm())
        {
            float NewArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetSpringArmLength, DeltaTime, ZoomInterpolationSpeed);
            SpringArm->TargetArmLength = NewArmLength;
        }
    }
}

void AAoAPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AAoAPlayerController::Look);
        EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &AAoAPlayerController::Zoom);
    }
}

void AAoAPlayerController::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    TargetCameraYaw += LookAxisVector.X * MouseTurnRate * GetWorld()->GetDeltaSeconds();
    TargetCameraPitch += LookAxisVector.Y * MouseTurnRate * GetWorld()->GetDeltaSeconds();
    TargetCameraPitch = FMath::Clamp(TargetCameraPitch, -75.0f, 75.0f);
}

void AAoAPlayerController::Zoom(const FInputActionValue& Value)
{
    const float ZoomAxisValue = Value.Get<float>();

    TargetSpringArmLength -= ZoomAxisValue * ZoomStep;
    TargetSpringArmLength = FMath::Clamp(TargetSpringArmLength, MinZoomDistance, MaxZoomDistance);
}

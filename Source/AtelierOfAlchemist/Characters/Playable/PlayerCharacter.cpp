// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "../../DataAssets/CharacterDataAsset.h"
#include "../../PlayerRuntimeData.h"
#include "Engine/GameInstance.h"
#include "../../GuildMemberManagerSubsystem.h"
#include "../StatComponent.h"
#include "../../Object/InteractableInterface.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 350.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Type = ECharacterType::Player;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &APlayerCharacter::Zoom);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(IA_Collect, ETriggerEvent::Triggered, this, &APlayerCharacter::CollectItem);
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Default, 0);
		}
	}

	UStatComponent* MyStatComp = GetStatComponent();

	UCharacterDataAsset* MyDataAsset = GetCharacterData();

	UGuildMemberManagerSubsystem* GuildManager = GetGameInstance()->GetSubsystem<UGuildMemberManagerSubsystem>();

	if (MyStatComp && MyDataAsset && GuildManager)
	{
		UPlayerRuntimeData* MyRuntimeData = GuildManager->GetPlayerRuntimeData(MyDataAsset->GetFName());

		if (MyRuntimeData)
		{
			MyStatComp->Initialize(MyRuntimeData);
		}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Spd
	float CurrentSpeed = GetCharacterMovement()->GetLastUpdateVelocity().Size();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			0.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Current Speed: %.2f"), CurrentSpeed)
		);
	}

	if (Controller != nullptr)
	{
		// 카메라 관련
		UpdateCameraLook(DeltaTime);
		UpdateCameraZoom(DeltaTime);

		// 캐릭터 이동 관련
		UpdateCharacterRotate(DeltaTime);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	CameraYaw += LookAxisVector.X * MouseSensitivity * GetWorld()->GetDeltaSeconds();
	CameraPitch += LookAxisVector.Y * MouseSensitivity * GetWorld()->GetDeltaSeconds();
	CameraPitch = FMath::Clamp(CameraPitch, -75.0f, 75.0f);
}

void APlayerCharacter::UpdateCameraLook(float DeltaTime)
{
	const FRotator CurrentRotation = Controller->GetControlRotation();
	const FRotator TargetRotation = FRotator(CameraPitch, CameraYaw, 0.0f);
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, CameraInterpSpeed);

	Controller->SetControlRotation(NewRotation);
}

void APlayerCharacter::Zoom(const FInputActionValue& Value)
{
	const float ZoomVector = Value.Get<float>();

	TargetSpringArmLength += ZoomVector * ZoomRate;
	TargetSpringArmLength = FMath::Clamp(TargetSpringArmLength, MinSpringArmLength, MaxSpringArmLength);
}

void APlayerCharacter::UpdateCameraZoom(float DeltaTime)
{
	if (SpringArm)
	{
		float CurrentSpringArmLength = SpringArm->TargetArmLength;
		float NewSpringArmLength = FMath::FInterpTo(CurrentSpringArmLength, TargetSpringArmLength, DeltaTime, ZoomInterpSpeed);

		SpringArm->TargetArmLength = NewSpringArmLength;
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MoveVector.Y);
		AddMovementInput(RightDirection, MoveVector.X);
	}
}

void APlayerCharacter::CollectItem()
{
	if (CurrentInteractObject)
	{
		if (CurrentInteractObject->Implements<UInteractableInterface>())
		{
			IInteractableInterface::Execute_Interact(CurrentInteractObject, this);
		}
	}
}

void APlayerCharacter::UpdateCharacterRotate(float DeltaTime)
{
	const FVector CurrentVelocity = GetCharacterMovement()->GetLastUpdateVelocity();

	if (!CurrentVelocity.IsNearlyZero())
	{
		const FRotator TargetRotation = CurrentVelocity.Rotation();

		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, CharacterRotationInterpSpeed);
		SetActorRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
	}
}

void APlayerCharacter::SetInteractObject(AActor* InteractObject)
{
	CurrentInteractObject = InteractObject;
	OnInteract.Broadcast(InteractObject);
}

void APlayerCharacter::ClearInteractObject(AActor* InteractObject)
{
	if (CurrentInteractObject == InteractObject)
	{
		CurrentInteractObject = nullptr;
		OnInteract.Broadcast(nullptr);
	}
}

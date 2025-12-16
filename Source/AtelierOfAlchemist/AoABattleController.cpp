// Fill out your copyright notice in the Description page of Project Settings.


#include "AoABattleController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BattleGameMode.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "CineCameraActor.h"


void AAoABattleController::BeginPlay()
{
	Super::BeginPlay();

	BattleGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());

	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);
	SetShowMouseCursor(false);
	SetInputMode_Main();
	SetMainCamera();
}

void AAoABattleController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &AAoABattleController::Input_Attack);
		EnhancedInputComponent->BindAction(IA_Skill, ETriggerEvent::Started, this, &AAoABattleController::Input_Skill);
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &AAoABattleController::Input_Run);

		EnhancedInputComponent->BindAction(IA_Skill_1, ETriggerEvent::Started, this, &AAoABattleController::Input_Skill_1);
		EnhancedInputComponent->BindAction(IA_Skill_2, ETriggerEvent::Started, this, &AAoABattleController::Input_Skill_2);
		EnhancedInputComponent->BindAction(IA_Skill_3, ETriggerEvent::Started, this, &AAoABattleController::Input_Skill_3);
	}
}

void AAoABattleController::SetInputMode_Main()
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(IMC_Battle_Main, 0);
	}
}

void AAoABattleController::SetInputMode_Skill()
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(IMC_Battle_Skill, 0);
	}
}

void AAoABattleController::SetMainCamera()
{
	TArray<AActor*> MainCamera;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "BattleMainCamera", MainCamera);

	if (MainCamera.Num() > 0)
	{
		SetViewTargetWithBlend(MainCamera[0], 0.0f);
	}
	else UE_LOG(LogTemp, Warning, TEXT("NO CAMERA!"));
}

void AAoABattleController::Input_Attack()
{
	BattleGameMode->ProcessPlayerAction(0);
}

void AAoABattleController::Input_Skill()
{
	BattleGameMode->ProcessPlayerAction(1);
}

void AAoABattleController::Input_Run()
{
	BattleGameMode->ProcessPlayerAction(2);
}

void AAoABattleController::Input_Skill_1()
{
	BattleGameMode->ProcessSkillSelection(0);
}

void AAoABattleController::Input_Skill_2()
{
	BattleGameMode->ProcessSkillSelection(1);
}

void AAoABattleController::Input_Skill_3()
{
	BattleGameMode->ProcessSkillSelection(2);
}

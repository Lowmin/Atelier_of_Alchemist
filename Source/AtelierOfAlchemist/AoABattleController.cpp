// Fill out your copyright notice in the Description page of Project Settings.


#include "AoABattleController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BattleGameMode.h"

void AAoABattleController::BeginPlay()
{
	Super::BeginPlay();

	BattleGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedSubsystem->AddMappingContext(IMC_Battle, 0);
	}

	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);
	SetShowMouseCursor(false);
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

void AAoABattleController::Input_Attack()
{
	BattleGameMode->PlayerAction(0);
}

void AAoABattleController::Input_Skill()
{
	BattleGameMode->PlayerAction(1);
}

void AAoABattleController::Input_Run()
{
	BattleGameMode->PlayerAction(2);
}

void AAoABattleController::Input_Skill_1()
{

}

void AAoABattleController::Input_Skill_2()
{

}

void AAoABattleController::Input_Skill_3()
{

}

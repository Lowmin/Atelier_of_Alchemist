// Fill out your copyright notice in the Description page of Project Settings.


#include "AoABattleController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BattleGameMode.h"

void AAoABattleController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedSubsystem->AddMappingContext(IMC_Battle, 0);
	}
}

void AAoABattleController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Select, ETriggerEvent::Started, this, &AAoABattleController::Select);
		EnhancedInputComponent->BindAction(IA_Cancel, ETriggerEvent::Started, this, &AAoABattleController::Cancel);
	}
}

void AAoABattleController::Select()
{
	TSharedPtr<SWidget> FocusedWidget = FSlateApplication::Get().GetKeyboardFocusedWidget();
	if (FocusedWidget.IsValid())
	{
		FKeyEvent KeyEvent(
			EKeys::Enter,
			FModifierKeysState(),
			0,
			false,
			0,
			0
		);

		FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);
		FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
	}
}

void AAoABattleController::Cancel()
{
	if (ABattleGameMode* BattleGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode()))
	{
		BattleGameMode->Undo();
	}
}
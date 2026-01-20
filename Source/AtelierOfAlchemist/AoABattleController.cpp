// Fill out your copyright notice in the Description page of Project Settings.


#include "AoABattleController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BattleGameMode.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "CineCameraActor.h"
#include "Characters/Battle/BattleUnit.h"
#include "DataAssets/SkillDataAsset.h"
#include "Characters/Battle/PlayerBattleUnit.h"


void AAoABattleController::BeginPlay()
{
	Super::BeginPlay();

	BattleGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	SetShowMouseCursor(false);

	SetMainCamera();
	SetInputMode_Main();
}

void AAoABattleController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &AAoABattleController::Input_Attack);
		EnhancedInputComponent->BindAction(IA_Skill, ETriggerEvent::Started, this, &AAoABattleController::Input_Skill);
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &AAoABattleController::Input_Run);
		EnhancedInputComponent->BindAction(IA_UseItem, ETriggerEvent::Started, this, &AAoABattleController::Input_UseItem);
		EnhancedInputComponent->BindAction(IA_Dodge, ETriggerEvent::Started, this, &AAoABattleController::Input_Dodge);

		EnhancedInputComponent->BindAction(IA_Skill_1, ETriggerEvent::Started, this, &AAoABattleController::Input_Skill_1);
		EnhancedInputComponent->BindAction(IA_Skill_2, ETriggerEvent::Started, this, &AAoABattleController::Input_Skill_2);
		EnhancedInputComponent->BindAction(IA_Skill_3, ETriggerEvent::Started, this, &AAoABattleController::Input_Skill_3);

		EnhancedInputComponent->BindAction(IA_Left, ETriggerEvent::Started, this, &AAoABattleController::Input_Left);
		EnhancedInputComponent->BindAction(IA_Right, ETriggerEvent::Started, this, &AAoABattleController::Input_Right);
		EnhancedInputComponent->BindAction(IA_Confirm, ETriggerEvent::Started, this, &AAoABattleController::Input_Confirm);

		EnhancedInputComponent->BindAction(IA_Cancel, ETriggerEvent::Started, this, &AAoABattleController::Input_Cancel);
	}
}

void AAoABattleController::SetInputMode_Main()
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(IMC_Battle_Main, 0);
		IsTargetingMode = false;
	}
}

void AAoABattleController::SetInputMode_Skill()
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(IMC_Battle_Skill, 0);
		IsTargetingMode = false;
	}
}

void AAoABattleController::SetInputMode_Targeting()
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(IMC_Battle_Targeting, 0);
		IsTargetingMode = true;
	}
}

void AAoABattleController::SetInputMode_EnemyTurn()
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(IMC_Battle_EnemyTurn, 0);
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
}

void AAoABattleController::StartTargetingMode(USkillDataAsset* SelectedSkill)
{
	CachedSkill = SelectedSkill;
	PossibleTargets.Empty();

	TArray<ABattleUnit*> AllUnits = BattleGameMode->GetAllUnits();

	for (ABattleUnit* Unit : AllUnits)
	{
		if (Unit->GetCurHealth() <= 0) continue;
		
		bool IsAlly = (Unit->Type == ECharacterType::Player);

		switch (CachedSkill->Target)
		{
		case ESkillTarget::Enemy:
			if (!IsAlly) PossibleTargets.Add(Unit);
			break;
		case ESkillTarget::Ally:
			if (IsAlly) PossibleTargets.Add(Unit);
			break;
		case ESkillTarget::Self:
			if (Unit == BattleGameMode->GetCurrentUnit()) PossibleTargets.Add(Unit);
			break;
		}
	}

	if (PossibleTargets.IsEmpty())
	{
		Input_Cancel();
		return;
	}

	TargetIndex = 0;
	SetInputMode_Targeting();
	UpdateTargetWidget();
}

void AAoABattleController::Input_Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Press A"));
	BattleGameMode->ProcessPlayerAction(0);
}

void AAoABattleController::Input_Skill()
{
	UE_LOG(LogTemp, Warning, TEXT("Press S"));
	BattleGameMode->ProcessPlayerAction(1);

	SetInputMode_Skill();
}

void AAoABattleController::Input_Run()
{
	BattleGameMode->ProcessPlayerAction(2);
}

void AAoABattleController::Input_UseItem()
{

}

void AAoABattleController::Input_Dodge()
{
	ABattleGameMode* BattleGM = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!BattleGM) return;

	ABattleUnit* Attacker = BattleGM->GetCurrentUnit();
	if (!Attacker) return;

	const TArray<ABattleUnit*>& Targets = Attacker->GetCachedTargets();

	for (ABattleUnit* TargetUnit : Targets)
	{
		if (APlayerBattleUnit* PlayerTarget = Cast<APlayerBattleUnit>(TargetUnit))
		{
			PlayerTarget->Dodge();
		}
	}
}

void AAoABattleController::Input_Skill_1()
{
	BattleGameMode->ProcessSkillSelection(1);
}

void AAoABattleController::Input_Skill_2()
{
	BattleGameMode->ProcessSkillSelection(2);
}

void AAoABattleController::Input_Skill_3()
{
	BattleGameMode->ProcessSkillSelection(3);
}

void AAoABattleController::Input_Left()
{
	if (PossibleTargets.IsEmpty()) return;

	TargetIndex--;

	if (TargetIndex < 0)
	{
		TargetIndex = PossibleTargets.Num() - 1;
	}

	UpdateTargetWidget();
}

void AAoABattleController::Input_Right()
{
	if (PossibleTargets.IsEmpty()) return;

	TargetIndex++;

	if (TargetIndex >= PossibleTargets.Num())
	{
		TargetIndex = 0;
	}

	UpdateTargetWidget();
}

void AAoABattleController::Input_Confirm()
{
	TArray<ABattleUnit*> FinalTargets;

	if (CachedSkill->Scope == ESkillScope::All)
	{
		FinalTargets = PossibleTargets;
	}
	else
	{
		if (PossibleTargets.IsValidIndex(TargetIndex))
		{
			FinalTargets.Add(PossibleTargets[TargetIndex]);
		}
	}

	for (ABattleUnit* Unit : PossibleTargets)
	{
		if (Unit) Unit->SetTargetSelect(false);
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
	}

	if (BattleGameMode && BattleGameMode->GetCurrentUnit())
	{
		BattleGameMode->GetCurrentUnit()->BattleAction_UseSkill(CachedSkill, FinalTargets);
	}

	IsTargetingMode = false;
	CachedSkill = nullptr;
	PossibleTargets.Empty();
}

void AAoABattleController::Input_Cancel()
{
	if (IsTargetingMode)
	{
		for (ABattleUnit* Unit : PossibleTargets)
		{
			if (Unit) Unit->SetTargetSelect(false);
		}

		IsTargetingMode = false;

		if (BattleGameMode) BattleGameMode->UndoLastAction();

		SetInputMode_Main();
	}
	else
	{
		for (ABattleUnit* Unit : PossibleTargets)
		{
			if (Unit) Unit->SetTargetSelect(false);
		}

		if (BattleGameMode) BattleGameMode->UndoLastAction();

		SetInputMode_Main();
	}
}

void AAoABattleController::UpdateTargetWidget()
{
	for (ABattleUnit* Unit : PossibleTargets)
	{
		if (Unit) Unit->SetTargetSelect(false);
	}

	if (CachedSkill && CachedSkill->Scope == ESkillScope::All)
	{
		for (ABattleUnit* Unit : PossibleTargets)
		{
			if (Unit) Unit->SetTargetSelect(true);
		}
	}
	else
	{
		if (PossibleTargets.IsValidIndex(TargetIndex))
		{
			PossibleTargets[TargetIndex]->SetTargetSelect(true);
		}
	}
}

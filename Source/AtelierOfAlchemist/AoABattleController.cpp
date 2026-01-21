#include "AoABattleController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BattleGameMode.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Battle/BattleUnit.h"
#include "Characters/StatComponent.h"
#include "DataAssets/SkillDataAsset.h"
#include "DataAssets/SkillListComponent.h"

void AAoABattleController::BeginPlay()
{
	Super::BeginPlay();

	BattleGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	SetShowMouseCursor(true);

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
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		if (IMC_Battle_Main) Subsystem->AddMappingContext(IMC_Battle_Main, 0);
	}
	IsTargetingMode = false;
}

void AAoABattleController::SetInputMode_Skill()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		if (IMC_Battle_Skill) Subsystem->AddMappingContext(IMC_Battle_Skill, 0);
	}
	IsTargetingMode = false;
}

void AAoABattleController::SetInputMode_Targeting()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		if (IMC_Battle_Targeting) Subsystem->AddMappingContext(IMC_Battle_Targeting, 0);
	}
	IsTargetingMode = true;
}

void AAoABattleController::SetInputMode_EnemyTurn()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		if (IMC_Battle_EnemyTurn) Subsystem->AddMappingContext(IMC_Battle_EnemyTurn, 0);
	}
}

void AAoABattleController::SetMainCamera()
{
	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("BattleMainCamera"), Cameras);

	if (Cameras.Num() > 0)
	{
		SetViewTargetWithBlend(Cameras[0], 0.0f);
	}
}

void AAoABattleController::StartPlayerTurn()
{
	IsTargetingMode = false;
	CachedSkill = nullptr;
	PossibleTargets.Empty();
	TargetIndex = 0;

	if (BattleGameMode)
	{
		for (ABattleUnit* Unit : BattleGameMode->GetAllUnits())
		{
			if (Unit) Unit->SetTargetSelected(false);
		}
	}

	SetInputMode_Main();
}

void AAoABattleController::StartTargetingMode(USkillDataAsset* SelectedSkill)
{
	if (!SelectedSkill || !BattleGameMode) return;

	CachedSkill = SelectedSkill;
	PossibleTargets.Empty();

	TArray<ABattleUnit*> AllUnits = BattleGameMode->GetAllUnits();


	for (ABattleUnit* Unit : AllUnits)
	{
		if (!Unit || Unit->GetStatComponent()->GetCurrentHealth() <= 0) continue;

		bool bIsSelf = (Unit == BattleGameMode->GetCurrentUnit());
		bool bIsAlly = Unit->IsPlayerTeam();

		if (CachedSkill->Target == ESkillTarget::Enemy && !bIsAlly)
		{
			PossibleTargets.Add(Unit);
		}
		else if (CachedSkill->Target == ESkillTarget::Ally && bIsAlly)
		{
			PossibleTargets.Add(Unit);
		}
	}

	TargetIndex = 0;
	SetInputMode_Targeting();
	UpdateTargetWidget();
}

void AAoABattleController::Input_Attack()
{

	if (!BattleGameMode) return;

	ABattleUnit* CurrentUnit = BattleGameMode->GetCurrentUnit();
	if (!CurrentUnit) return;

	if (CurrentUnit->SkillComponent)
	{
		const auto& Skills = CurrentUnit->SkillComponent->GetSkillList();

		if (Skills.Num() > 0 && Skills[0])
		{
			StartTargetingMode(Skills[0]);
		}
	}
}

void AAoABattleController::Input_Skill()
{
	SetInputMode_Skill();
}

void AAoABattleController::Input_Run()
{
}

void AAoABattleController::Input_UseItem()
{
}

void AAoABattleController::Input_Dodge()
{
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

void AAoABattleController::Input_Left()
{
	if (!IsTargetingMode || PossibleTargets.IsEmpty()) return;

	TargetIndex = (TargetIndex + 1) % PossibleTargets.Num();

	UpdateTargetWidget();
}

void AAoABattleController::Input_Right()
{
	if (!IsTargetingMode || PossibleTargets.IsEmpty()) return;

	TargetIndex--;
	if (TargetIndex < 0) TargetIndex = PossibleTargets.Num() - 1;
	
	UpdateTargetWidget();
}

void AAoABattleController::Input_Confirm()
{
	if (!IsTargetingMode || PossibleTargets.IsEmpty()) return;
	if (!BattleGameMode) return;

	ABattleUnit* Source = BattleGameMode->GetCurrentUnit();
	ABattleUnit* Target = PossibleTargets[TargetIndex];

	if (Source && Target && CachedSkill)
	{
		for (ABattleUnit* Unit : PossibleTargets)
		{
			if (Unit) Unit->SetTargetSelected(false);
		}

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
		}
		IsTargetingMode = false;

		BattleGameMode->ExecuteAction(Source, Target, CachedSkill);
	}
}

void AAoABattleController::Input_Cancel()
{
	if (IsTargetingMode)
	{
		for (ABattleUnit* Unit : PossibleTargets)
		{
			if (Unit) Unit->SetTargetSelected(false);
		}
		SetInputMode_Main();
	}
	else
	{
		SetInputMode_Main();
	}
}

void AAoABattleController::UpdateTargetWidget()
{
	for (int32 i = 0; i < PossibleTargets.Num(); i++)
	{
		if (PossibleTargets[i])
		{
			PossibleTargets[i]->SetTargetSelected(i == TargetIndex);
		}
	}
}
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

}

void AAoABattleController::SetInputMode_Skill()
{

}

void AAoABattleController::SetInputMode_Targeting()
{

}

void AAoABattleController::SetInputMode_EnemyTurn()
{

}

void AAoABattleController::SetMainCamera()
{

}

void AAoABattleController::StartTargetingMode(USkillDataAsset* SelectedSkill)
{

}

void AAoABattleController::Input_Attack()
{

}

void AAoABattleController::Input_Skill()
{

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

}

void AAoABattleController::Input_Right()
{

}

void AAoABattleController::Input_Confirm()
{

}

void AAoABattleController::Input_Cancel()
{

}

void AAoABattleController::UpdateTargetWidget()
{

}

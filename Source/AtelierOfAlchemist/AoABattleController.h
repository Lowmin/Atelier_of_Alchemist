// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AoABattleController.generated.h"

class UInputMappingContext;
class UInputAction;
class ABattleGameMode;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API AAoABattleController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetInputMode_Main();
	void SetInputMode_Skill();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY()
	TObjectPtr<ABattleGameMode> BattleGameMode;

	// InputMappingContext
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Battle_Main;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Battle_Skill;

	// 공통 조작
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Cancel;

	// BattleUI 조작
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Attack;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Skill;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Run;

	// SkillList 조작
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Skill_1;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Skill_2;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Skill_3;

	void Input_Attack();
	void Input_Skill();
	void Input_Run();

	void Input_Skill_1();
	void Input_Skill_2();
	void Input_Skill_3();

	void SetBattleCamera();
};

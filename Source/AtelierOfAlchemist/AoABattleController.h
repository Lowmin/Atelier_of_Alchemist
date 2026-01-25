#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AoABattleController.generated.h"

class UInputMappingContext;
class UInputAction;
class ABattleGameMode;
class USkillDataAsset;
class ABattleUnit;
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
	void SetInputMode_Targeting();
	void SetInputMode_EnemyTurn();

	void SetMainCamera();

	void StartPlayerTurn();
	void StartTargetingMode(USkillDataAsset* SelectedSkill);

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
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Battle_Targeting;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Battle_EnemyTurn;

	// 공통 조작
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Cancel;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Confirm;

	// BattleUI 조작
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Attack;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Skill;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Run;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_UseItem;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Dodge;

	// SkillList 조작
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Skill_1;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Skill_2;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Skill_3;

	// Target 조작
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Left;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Right;

	// 배틀 UI 관련
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UBattleMainWidget> BattleUIClass;
	UPROPERTY()
	TObjectPtr<class UBattleMainWidget> BattleUIInstance;

public:
	void Input_Attack();
	void Input_Skill();
	void Input_Run();
	void Input_UseItem();
	void Input_Dodge();

	void Input_Skill_1();
	void Input_Skill_2();
	void Input_Skill_3();

	void Input_Left();
	void Input_Right();
	void Input_Confirm();
	void Input_Cancel();

private:
	bool IsTargetingMode = false;
	int32 TargetIndex = 0;

	UPROPERTY()
	TObjectPtr<USkillDataAsset> CachedSkill;

	UPROPERTY()
	TArray<ABattleUnit*> PossibleTargets;

	void UpdateTargetWidget();
};

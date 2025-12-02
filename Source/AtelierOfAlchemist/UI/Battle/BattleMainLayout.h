// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleMainLayout.generated.h"

class UWidgetSwitcher;
class UBattleUI;
// class USkillListUI;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UBattleMainLayout : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowBattleUI();	// 행동 선택
	void ShowSkillUI();		// 스킬 리스트
	void ShowTargetUI();	// 타겟 지정

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBattleUI> BattleUI;

	/*UPROPERTY(meta = (BindWidget))
	TObjectPtr<USkillListUI> SkillListMenu;*/
};

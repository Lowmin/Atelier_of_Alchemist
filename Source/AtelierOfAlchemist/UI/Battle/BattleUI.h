// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUI.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UBattleUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowUI();
	void HideUI();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Attack;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Skill;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Run;

	UFUNCTION()
	void OnAttack();
	void OnSkill();
	void OnRun();
};

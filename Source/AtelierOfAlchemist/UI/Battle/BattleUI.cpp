// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleUI.h"
#include "Components/Button.h"

void UBattleUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Attack) Btn_Attack->OnClicked.AddDynamic(this, &UBattleUI::OnAttack);
	if (Btn_Skill) Btn_Skill->OnClicked.AddDynamic(this, &UBattleUI::OnSkill);
	if (Btn_Run) Btn_Run->OnClicked.AddDynamic(this, &UBattleUI::OnRun);
}

void UBattleUI::ShowUI()
{

}

void UBattleUI::HideUI()
{

}

void UBattleUI::OnAttack()
{

}

void UBattleUI::OnSkill()
{

}

void UBattleUI::OnRun()
{

}

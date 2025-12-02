// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleMainLayout.h"
#include "Components/WidgetSwitcher.h"
#include "BattleUI.h"

void UBattleMainLayout::ShowBattleUI()
{
	if (MenuSwitcher)
	{
		MenuSwitcher->SetActiveWidgetIndex(0);
	}
	if (BattleUI)
	{
		BattleUI->ShowUI();
	}
}

void UBattleMainLayout::ShowSkillUI()
{

}

void UBattleMainLayout::ShowTargetUI()
{

}

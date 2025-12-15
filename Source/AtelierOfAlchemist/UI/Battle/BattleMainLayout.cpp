// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleMainLayout.h"
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"
#include "PlayerStatusSlot.h"
#include "BattleTurnWidget.h"
#include "../../PlayerRuntimeData.h"
#include "BattleUI.h"

void UBattleMainLayout::HideBattleUI()
{
	if (BattleUI)
	{
		BattleUI->HideUI();
	}
}

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

void UBattleMainLayout::InitStatusSlot(TArray<UPlayerRuntimeData*>& PartyDataList)
{
	if (!StatusSlotBox || !PlayerStatusSlotClass) return;

	StatusSlotBox->ClearChildren();

	for (UPlayerRuntimeData* MemberData : PartyDataList)
	{
		if (MemberData)
		{
			UPlayerStatusSlot* NewSlot = CreateWidget<UPlayerStatusSlot>(this, PlayerStatusSlotClass);

			if (NewSlot)
			{
				NewSlot->InitializeSlot(MemberData);
				StatusSlotBox->AddChildToHorizontalBox(NewSlot);
				NewSlot->UpdateUI(MemberData->GetCurrentHealth(), MemberData->GetMaxHealth());
			}
		}
	}
}

void UBattleMainLayout::UpdateTurnSlotBar(const TArray<ACharacterBase*>& TurnQueue)
{
	if (TurnWidget)
	{
		TurnWidget->UpdateTurnOrder(TurnQueue);
	}
}



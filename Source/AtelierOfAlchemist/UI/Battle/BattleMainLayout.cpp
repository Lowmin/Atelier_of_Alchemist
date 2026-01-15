// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleMainLayout.h"
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "PlayerStatusSlot.h"
#include "BattleTurnWidget.h"
#include "SkillSlotWidget.h"
#include "../../PlayerRuntimeData.h"
#include "BattleUI.h"

void UBattleMainLayout::HideBattleUI()
{
	if (MenuSwitcher)
	{
		MenuSwitcher->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UBattleMainLayout::ShowBattleUI()
{
	if (MenuSwitcher)
	{
		MenuSwitcher->SetVisibility(ESlateVisibility::Visible);
		MenuSwitcher->SetActiveWidget(BattleUI);
	}
}

void UBattleMainLayout::ShowSkillUI()
{
	if (MenuSwitcher)
	{
		MenuSwitcher->SetVisibility(ESlateVisibility::Visible);
		MenuSwitcher->SetActiveWidget(SkillSelectBox);
	}
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

void UBattleMainLayout::UpdateTurnSlotBar(const TArray<ABattleUnit*>& TurnQueue)
{
	if (TurnWidget)
	{
		TurnWidget->UpdateTurnOrder(TurnQueue);
	}
}

void UBattleMainLayout::InitSkillList(const TArray<USkillDataAsset*>& Skills)
{
	if (!SkillSelectBox || !SkillSlotClass) return;

	SkillSelectBox->ClearChildren();

	for (int32 i = 1; i < Skills.Num(); ++i)
	{
		if (USkillSlotWidget* SkillSlot = CreateWidget<USkillSlotWidget>(this, SkillSlotClass))
		{
			SkillSlot->InitSlot(Skills[i], i);
			SkillSelectBox->AddChild(SkillSlot);
		}
	}
}



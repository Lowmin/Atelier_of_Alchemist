// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTurnWidget.h"
#include "Components/VerticalBox.h"
#include "../../Characters/CharacterBase.h"
#include "BattleTurnSlot.h"

void UBattleTurnWidget::UpdateTurnOrder(const TArray<ACharacterBase*>& TurnQueue)
{
	TurnSlotBox->ClearChildren();

	for (ACharacterBase* Unit : TurnQueue)
	{
		if (Unit->GetCurHealth() <= 0) continue;

		UBattleTurnSlot* NewSlot = CreateWidget<UBattleTurnSlot>(this, BattleTurnSlotClass);

		if (NewSlot)
		{
			NewSlot->InitSlot(Unit);
			TurnSlotBox->AddChildToVerticalBox(NewSlot);
		}
	}
}

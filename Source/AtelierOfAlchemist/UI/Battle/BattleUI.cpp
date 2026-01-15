// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleUI.h"
#include "Components/Button.h"
#include "../../BattleGameMode.h"

void UBattleUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetWorld()) BattleGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());
}

void UBattleUI::ShowUI()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UBattleUI::HideUI()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

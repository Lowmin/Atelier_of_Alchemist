// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatusWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UEnemyStatusWidget::UpdateHp(float CurrentHp, float MaxHp)
{
	if (MaxHp >= 0.0f)
	{
		float Percent = CurrentHp / MaxHp;
		HpBar->SetPercent(Percent);
	}
}

void UEnemyStatusWidget::SetLevel(int32 Level)
{
	FString LevelString = FString::Printf(TEXT("%d"), Level);
	LevelText->SetText(FText::FromString(LevelString));
}

void UEnemyStatusWidget::SetName(FText Name)
{
	NameText->SetText(Name);
}
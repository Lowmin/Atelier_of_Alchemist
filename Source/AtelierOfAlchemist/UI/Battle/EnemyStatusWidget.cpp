// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatusWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "../../Characters/StatComponent.h"

void UEnemyStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HpBar)
	{
		HpBar->SetPercent(1.0f);
	}
}

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

void UEnemyStatusWidget::BindStatComponent(UStatComponent* StatComp)
{
	StatComp->OnHealthChanged.AddDynamic(this, &UEnemyStatusWidget::UpdateHp);
}

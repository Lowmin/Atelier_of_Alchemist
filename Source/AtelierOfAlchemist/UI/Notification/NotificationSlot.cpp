// Fill out your copyright notice in the Description page of Project Settings.


#include "NotificationSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "NotificationData.h"
#include "Animation/WidgetAnimation.h"

void UNotificationSlot::Initialize(const FNotificationData& Data)
{
	NotificationText->SetText(Data.Message);

	if (Data.Icon.IsNull())
	{
		NotificationIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		NotificationIcon->SetBrushFromSoftTexture(Data.Icon);
		NotificationIcon->SetVisibility(ESlateVisibility::Visible);
	}

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &UNotificationSlot::OnFadeOutFinished, Data.Duration, false);
}


void UNotificationSlot::OnFadeOutFinished()
{
	float AnimDuration = 1.0f;

	if (FadeOutAnim)
	{
		AnimDuration = FadeOutAnim->GetEndTime();
		PlayAnimation(FadeOutAnim);
	}

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &UNotificationSlot::RemoveFromParent, AnimDuration, false);
}

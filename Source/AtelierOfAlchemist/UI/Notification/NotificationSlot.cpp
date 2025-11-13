// Fill out your copyright notice in the Description page of Project Settings.


#include "NotificationSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "NotificationData.h"

void UNotificationSlot::Initialize(const FNotificationData& Data)
{
	NotificationText->SetText(Data.Message);
}


void UNotificationSlot::OnFadeOutFinished()
{

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "NotificationContainer.h"
#include "NotificationSlot.h"
#include "Components/VerticalBox.h"

void UNotificationContainer::AddNotification(const FNotificationData& Data)
{
	if (!NotificationSlot)
	{
		return;
	}

	if (!NotificationBox)
	{
		return;
	}

	UNotificationSlot* NewSlot = CreateWidget<UNotificationSlot>(this, NotificationSlot);
	if (NewSlot)
	{
		NotificationBox->AddChildToVerticalBox(NewSlot);
		NewSlot->Initialize(Data);
	}
}

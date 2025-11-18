// Fill out your copyright notice in the Description page of Project Settings.


#include "NotificationManagerSubsystem.h"
#include "UI/Notification/NotificationContainer.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"

void UNotificationManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (ContainerWidgetClass)
	{
		ULocalPlayer* LocalPlayer = GetLocalPlayer();
		if (!LocalPlayer) return;

		APlayerController* PC = LocalPlayer->GetPlayerController(GetWorld());

		if (PC)
		{
			ContainerWidgetInstance = CreateWidget<UNotificationContainer>(PC, ContainerWidgetClass);
			if (ContainerWidgetInstance)
			{
				ContainerWidgetInstance->AddToViewport(10);
			}
		}
	}
}

void UNotificationManagerSubsystem::ShowNotification(const FNotificationData& Data)
{
	if (ContainerWidgetInstance)
	{
		ContainerWidgetInstance->AddNotification(Data);
	}
}

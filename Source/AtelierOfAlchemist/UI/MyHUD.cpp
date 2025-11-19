// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "../Object/CollectingObject.h"
#include "../DataAssets/ItemDataAsset.h"
#include "Notification/NotificationContainer.h"
#include "MainUI.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainUIClass)
	{
		MainUIInstance = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
		if (MainUIInstance)
		{
			MainUIInstance->AddToViewport();
		}
	}

	if (NotificationContainerClass)
	{
		// PlayerController를 Owner로 생성
		NotificationContainer = CreateWidget<UNotificationContainer>(GetOwningPlayerController(), NotificationContainerClass);
		if (NotificationContainer)
		{
			// Z-Order를 높게(예: 10) 설정해서 다른 UI보다 위에 뜨게 함
			NotificationContainer->AddToViewport(10);
		}
	}
}

void AMyHUD::ShowNotification(const FNotificationData& Data)
{
	if (NotificationContainer)
	{
		NotificationContainer->AddNotification(Data);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationContainer.generated.h"

struct FNotificationData;
class UNotificationSlot;
/**
 *
 */
UCLASS()
class ATELIEROFALCHEMIST_API UNotificationContainer : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddNotification(const FNotificationData& Data);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> NotificationBox;

	UPROPERTY(EditDefaultsOnly, Category = "Notification")
	TSubclassOf<UNotificationSlot> NotificationSlot;
};

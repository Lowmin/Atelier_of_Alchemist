// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/Notification/NotificationData.h"
#include "NotificationManagerSubsystem.generated.h"

class UNotificationContainer;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UNotificationManagerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void ShowNotification(const FNotificationData& Data);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(EditAnywhere, Category = "Notification")
	TSubclassOf<UNotificationContainer> ContainerWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UNotificationContainer> ContainerWidgetInstance;
};

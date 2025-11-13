// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationSlot.generated.h"

struct FNotificationData;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UNotificationSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Initialize(const FNotificationData& Data);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> NotificationIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NotificationText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutAnim;

	FTimerHandle DestroyTimerHandle;

	void OnFadeOutFinished();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NotificationData.generated.h"

UENUM(BlueprintType)
enum class ENotificationType : uint8
{
	None,
	Item,
	Warning,
	Success
};

USTRUCT(BlueprintType)
struct FNotificationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENotificationType Type = ENotificationType::None;
};

/**
 *
 */

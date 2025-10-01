// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AoAPlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class ATELIEROFALCHEMIST_API AAoAPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAoAPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* IMC_Default;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Zoom;

	void Look(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);

private:
	float TargetCameraYaw{};
	float TargetCameraPitch{};

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraInterpSpeed = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MouseTurnRate = 45.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float TargetSpringArmLength = 800.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MinZoomDistance = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MaxZoomDistance = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomStep = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomInterpolationSpeed = 10.0f;
};

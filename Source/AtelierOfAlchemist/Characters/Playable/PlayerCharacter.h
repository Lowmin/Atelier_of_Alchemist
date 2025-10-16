// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartyMemberBase.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputComponent;
class UInputAction;
class UCharacterDataAsset;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, AActor*, InteractObject);

/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API APlayerCharacter : public APartyMemberBase
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 카메라 관련
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	float CameraYaw{};
	float CameraPitch{};

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MouseSensitivity = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraInterpSpeed = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float TargetSpringArmLength = 800.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MinSpringArmLength = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MaxSpringArmLength = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomRate = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomInterpSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Move")
	float CharacterRotationInterpSpeed = 10.0f;

	FVector2D LastMovementInput;

protected:
	// 조작 입력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> IMC_Default;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Zoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Move;

	// 조작 함수
	void Look(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);

	// 캐릭터 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Data")
	TObjectPtr<UCharacterDataAsset> CharacterDataAsset;

public:
	APlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void UpdateCameraLook(float DeltaTime);
	void UpdateCameraZoom(float DeltaTime);
	void UpdateCharacterRotate(float DeltaTime);

public:
	TObjectPtr<AActor> CurrentInteractObject;
	void SetInteractObject(AActor* InteractObject);
	void ClearInteractObject(AActor* InteractObject);

	UPROPERTY()
	FOnInteract OnInteract;
};

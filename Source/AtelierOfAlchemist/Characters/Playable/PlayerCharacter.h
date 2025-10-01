// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartyMemberBase.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API APlayerCharacter : public APartyMemberBase
{
	GENERATED_BODY()
	
private:
	// 카메라 관련
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

public:
	APlayerCharacter();

	USpringArmComponent* GetSpringArm() { return CameraBoom; };

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

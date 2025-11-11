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
class UMainUI;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_ToggleInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UMainUI> WBP_MainUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UMainUI> MainUIClass;

	void ToggleInventory();

private:
	bool bIsInventoryAnimPlay = false;
};

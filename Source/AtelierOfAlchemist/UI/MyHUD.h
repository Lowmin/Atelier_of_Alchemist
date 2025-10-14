// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UMainUI;
class AActor;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMainUI> MainUIClass;

	UPROPERTY()
	TObjectPtr<UMainUI> MainUIInstance;

	UFUNCTION()
	void OnInteract(AActor* InteractObject);
};

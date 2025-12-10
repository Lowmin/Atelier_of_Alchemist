// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleTurnWidget.generated.h"

class UBattleTurnSlot;
class ACharacterBase;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UBattleTurnWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateTurnOrder(const TArray<ACharacterBase*>& TurnQueue);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> TurnSlotBox;

	UPROPERTY(meta = (BindWidget))
	TSubclassOf<UBattleTurnSlot> BattleTurnSlotClass;
	
};

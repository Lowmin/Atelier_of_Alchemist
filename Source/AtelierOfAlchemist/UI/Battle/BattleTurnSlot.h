// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleTurnSlot.generated.h"

class UImage;
class ACharacterBase;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UBattleTurnSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitSlot(ACharacterBase* Unit);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Border;
};

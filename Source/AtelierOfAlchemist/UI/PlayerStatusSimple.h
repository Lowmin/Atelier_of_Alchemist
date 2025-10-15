// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatusSimple.generated.h"

class UImage;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UPlayerStatusSimple : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdatePlayerStatusSimple(TSoftObjectPtr<UTexture2D> CharacterImage, float MaxHP, float CurHP);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PlayerImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;
};

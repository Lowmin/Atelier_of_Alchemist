// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartyStatusInMenu.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UPartyStatusInMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Image;

	UPROPERTY(BLueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Health;

	UPROPERTY(BLueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Level;
};

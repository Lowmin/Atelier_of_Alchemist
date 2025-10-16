// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartyStatusInMenu.generated.h"

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
	TObjectPtr<UTexture2D> Image;

	UPROPERTY(BLueprintReadOnly, meta = (BindWidget))
	TObjectPtr<FText> Health;

	UPROPERTY(BLueprintReadOnly, meta = (BindWidget))
	TObjectPtr<FText> Level;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractObjectWidget.generated.h"

class UImage;
class UTextBlock;
/**
 *
 */
UCLASS()
class ATELIEROFALCHEMIST_API UInteractObjectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateWidgetInfo(TSoftObjectPtr<UTexture2D> ItemIcon, const FText& ItemName);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;
};

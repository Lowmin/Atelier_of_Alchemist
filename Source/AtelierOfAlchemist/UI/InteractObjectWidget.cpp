// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractObjectWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInteractObjectWidget::UpdateWidgetInfo(TSoftObjectPtr<UTexture2D> ItemIcon, const FText& ItemName)
{
	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(ItemIcon.LoadSynchronous());
	}

	if (ItemNameText)
	{
		ItemNameText->SetText(ItemName);
	}
}

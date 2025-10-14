// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "InteractObjectWidget.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();
	if (InteractObjectWidget)
	{
		InteractObjectWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainUI::ShowInteractWidget(TSoftObjectPtr<UTexture2D> ItemIcon, const FText& ItemName)
{
	if (InteractObjectWidget)
	{
		InteractObjectWidget->UpdateWidgetInfo(ItemIcon, ItemName);
		InteractObjectWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainUI::HideInteractWidget()
{
	if (InteractObjectWidget)
	{
		InteractObjectWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

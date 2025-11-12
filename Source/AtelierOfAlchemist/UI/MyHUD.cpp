// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "../Object/CollectingObject.h"
#include "../DataAssets/ItemDataAsset.h"
#include "MainUI.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainUIClass)
	{
		MainUIInstance = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
		if (MainUIInstance)
		{
			MainUIInstance->AddToViewport();
		}
	}
}
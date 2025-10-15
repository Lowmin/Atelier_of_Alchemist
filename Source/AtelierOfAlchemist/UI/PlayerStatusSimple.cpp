// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusSimple.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UPlayerStatusSimple::UpdatePlayerStatusSimple(TSoftObjectPtr<UTexture2D> CharacterImage, float MaxHP, float CurHP)
{
	if (PlayerImage)
	{
		PlayerImage->SetBrushFromTexture(CharacterImage.LoadSynchronous());
	}

	if (HPBar && MaxHP > 0)
	{
		HPBar->SetPercent(CurHP / MaxHP);
	}
	else
	{
		HPBar->SetPercent(0.0f);
	}
}

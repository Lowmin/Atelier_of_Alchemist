// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "../../PlayerRuntimeData.h"
#include "../../DataAssets/CharacterDataAsset.h"

void UPlayerStatusSlot::InitializeSlot(UPlayerRuntimeData* RuntimeData)
{
	if (!RuntimeData) return;

	CachedData = RuntimeData;

	UCharacterDataAsset* CharacterData = RuntimeData->GetCharacterDataAsset();
	if (CharacterData)
	{
		CharacterImage->SetBrushFromSoftTexture(CharacterData->CharacterImage);
		CharacterName->SetText(CharacterData->CharacterName);
		Speed->SetText(FText::AsNumber(CharacterData->BaseSpeed));

		RuntimeData->OnHealthChanged.AddDynamic(this, &UPlayerStatusSlot::UpdateUI);
	}
}

void UPlayerStatusSlot::UpdateUI(float CurrnetHp, float MaxHp)
{
	if (HpText)
	{
		FString HpString = FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(CurrnetHp), FMath::CeilToInt(MaxHp));
		HpText->SetText(FText::FromString(HpString));
	}
}

void UPlayerStatusSlot::NativeDestruct()
{
	Super::NativeDestruct();

	if (CachedData)
	{
		CachedData->OnHealthChanged.RemoveDynamic(this, &UPlayerStatusSlot::UpdateUI);
	}
}
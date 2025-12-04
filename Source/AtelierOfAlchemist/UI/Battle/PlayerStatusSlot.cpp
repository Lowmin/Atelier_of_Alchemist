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
		HpText->SetText(TEXT("%s / %s", CharacterData->BaseMaxHealth, RuntimeData->GetCurrentHealth));
	}
}

void UPlayerStatusSlot::UpdateUI(float CurrnetHp, float MaxHp)
{

}

void UPlayerStatusSlot::NativeDestruct()
{

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTurnSlot.h"
#include "Components/Image.h"
#include "../../DataAssets/CharacterDataAsset.h"
#include "../../Characters/CharacterBase.h"

void UBattleTurnSlot::InitSlot(ACharacterBase* Unit)
{
	if (!Unit) return;

	UCharacterDataAsset* CharacterData = Unit->GetCharacterData();
	CharacterIcon->SetBrushFromSoftTexture(CharacterData->CharacterImage);

	if (Unit->Type == ECharacterType::Player)
	{
		Border->SetColorAndOpacity(FLinearColor::Blue);
	}
	else
	{
		Border->SetColorAndOpacity(FLinearColor::Red);
	}
}

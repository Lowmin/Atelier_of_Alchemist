// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyMemberSlot.h"

#include "Components/Image.h"
#include "Components/Button.h"

#include "../../DataAssets/CharacterDataAsset.h"
#include "../../Characters/Playable/PlayerCharacter.h"

void UPartyMemberSlot::InitSlot(APlayerCharacter* InCharacter)
{
	if (!InCharacter->GetCharacterData()) return;

	LinkedCharacter = InCharacter;
	UCharacterDataAsset* CharacterData = InCharacter->GetCharacterData();

	Image_Portrait->SetBrushFromTexture(CharacterData->CharacterImage.LoadSynchronous());
	Button_Select->OnClicked.AddDynamic(this, &UPartyMemberSlot::OnButtonClicked);
}

void UPartyMemberSlot::OnButtonClicked()
{
	OnSlotSelected.Broadcast(LinkedCharacter);
}
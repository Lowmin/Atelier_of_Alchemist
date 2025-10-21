// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyMemberSlot.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "../DataAssets/CharacterDataAsset.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UPartyMemberSlot::InitializeSlot(APlayerCharacter* TargetCharacter)
{
	if (!TargetCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetCharacter is not found."));
		return;
	}

	WidgetPlayerCharacter = TargetCharacter;
	UCharacterDataAsset* CharacterDataAsset = TargetCharacter->GetCharacterData();


	if (WidgetPlayerCharacter.IsValid())
	{
		WidgetPlayerCharacter->OnHealthChanged.RemoveDynamic(this, &UPartyMemberSlot::OnHealthChange);
	}

	if (CharacterDataAsset == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataAsset is not found."));
		return;
	}

	CharacterImage->SetBrushFromSoftTexture(CharacterDataAsset->CharacterImage);
	TargetCharacter->OnHealthChanged.AddDynamic(this, &UPartyMemberSlot::OnHealthChange);
	OnHealthChange(TargetCharacter->GetCurHealth(), TargetCharacter->GetMaxHealth());
}

void UPartyMemberSlot::OnHealthChange(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		float Percent = (MaxHealth > 0) ? (CurrentHealth / MaxHealth) : 0.0f;
		HealthBar->SetPercent(Percent);
	}
}

void UPartyMemberSlot::NativeDestruct()
{
	if (WidgetPlayerCharacter.IsValid())
	{
		WidgetPlayerCharacter->OnHealthChanged.RemoveDynamic(this, &UPartyMemberSlot::OnHealthChange);
	}

	Super::NativeDestruct();
}

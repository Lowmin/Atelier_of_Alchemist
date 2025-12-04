// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyMemberSlot.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "../DataAssets/CharacterDataAsset.h"
#include "../PlayerRuntimeData.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "PlayerStatusSlot.h"

void UPartyMemberSlot::InitializeSlot(UPlayerRuntimeData* NewPlayerRuntimeData)
{
	UE_LOG(LogTemp, Warning, TEXT("InitializeSlot."));

	if (!NewPlayerRuntimeData || !NewPlayerRuntimeData->GetCharacterDataAsset())
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	if (PlayerRuntimeData.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("RuntimeData Found."));
		PlayerRuntimeData->OnHealthChanged.RemoveDynamic(this, &UPartyMemberSlot::OnHealthChange);
	}

	PlayerRuntimeData = NewPlayerRuntimeData;
	UCharacterDataAsset* StaticData = NewPlayerRuntimeData->GetCharacterDataAsset();

	if (CharacterImage)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharImage Found."));
		CharacterImage->SetBrushFromSoftTexture(StaticData->CharacterImage);
	}

	PlayerRuntimeData->OnHealthChanged.AddDynamic(this, &UPartyMemberSlot::OnHealthChange);

	OnHealthChange(PlayerRuntimeData->GetCurrentHealth(), PlayerRuntimeData->GetMaxHealth());

	SetVisibility(ESlateVisibility::Visible);
}

void UPartyMemberSlot::OnHealthChange(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		UE_LOG(LogTemp, Warning, TEXT("Health Change"));
		Percent = (MaxHealth > 0) ? (CurrentHealth / MaxHealth) : 0.0f;
		HealthBar->SetPercent(Percent);
	}
}

void UPartyMemberSlot::NativeDestruct()
{
	if (PlayerRuntimeData.IsValid())
	{
		PlayerRuntimeData->OnHealthChanged.RemoveDynamic(this, &UPartyMemberSlot::OnHealthChange);
	}

	Super::NativeDestruct();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyMemberList.h"
#include "../AoAGameInstance.h"
#include "PartyMemberSlot.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "../DataAssets/CharacterDataAsset.h"
#include "../GuildMemberManagerSubsystem.h"

void UPartyMemberList::NativeConstruct()
{
	GuildMemberManager = GetGameInstance()->GetSubsystem<UGuildMemberManagerSubsystem>();
	
	if (GuildMemberManager)
	{
		GuildMemberManager->OnPartyUpdate.AddDynamic(this, &UPartyMemberList::RefreshPartyList);
		RefreshPartyList();
	}

	Super::NativeConstruct();
}

void UPartyMemberList::NativeDestruct()
{
	if (GuildMemberManager)
	{
		GuildMemberManager->OnPartyUpdate.RemoveDynamic(this, &UPartyMemberList::RefreshPartyList);
	}

	Super::NativeDestruct();
}

void UPartyMemberList::RefreshPartyList()
{
	PartyList->ClearChildren();

	const TArray<FName>& PartyMemberIDs = GuildMemberManager->GetPartyMemberIDs();

	for (const FName& CharacterID : PartyMemberIDs)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s UI Init."), *CharacterID.ToString());
		UPlayerRuntimeData* CharData = GuildMemberManager->GetPlayerRuntimeData(CharacterID);

		if (CharData)
		{
			UE_LOG(LogTemp, Warning, TEXT("CharData Found."));
			UPartyMemberSlot* PartyMemberSlot = CreateWidget<UPartyMemberSlot>(this, BP_PartyMemberSlot);
			if (PartyMemberSlot)
			{
				UE_LOG(LogTemp, Warning, TEXT("PartyMemberSlot Found."));
				PartyMemberSlot->InitializeSlot(CharData);
				PartyList->AddChild(PartyMemberSlot);
			}
		}
	}
}

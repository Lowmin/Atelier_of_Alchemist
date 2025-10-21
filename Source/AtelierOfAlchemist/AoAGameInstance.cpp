// Fill out your copyright notice in the Description page of Project Settings.


#include "AoAGameInstance.h"
#include "Characters/Playable/PlayerCharacter.h"

void UAoAGameInstance::Init()
{
	Super::Init();

	if (GuildMemberIDs.IsEmpty())
	{
		if (!DefaultMemberID.IsNone())
		{
			AddGuildMember(DefaultMemberID);
			AddPartyMember(DefaultMemberID);
			AddGuildMember("DA_Sato");
			AddPartyMember("DA_Sato");
		}
	}
}

void UAoAGameInstance::AddGuildMember(FName CharacterID)
{
	GuildMemberIDs.AddUnique(CharacterID);
}

void UAoAGameInstance::AddPartyMember(FName CharacterID)
{
	if (PartyMemberIDs.Num() < 4 && GuildMemberIDs.Contains(CharacterID))
	{
		PartyMemberIDs.AddUnique(CharacterID);
		OnPartyUpdate.Broadcast();
	}
}

void UAoAGameInstance::RemovePartyMember(FName CharacterID)
{
	PartyMemberIDs.Remove(CharacterID);
	OnPartyUpdate.Broadcast();
}

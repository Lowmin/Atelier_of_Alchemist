// Fill out your copyright notice in the Description page of Project Settings.


#include "AoAGameInstance.h"
#include "Characters/Playable/PlayerCharacter.h"

void UAoAGameInstance::AddPartyMember(FName MemberID)
{
	arrPartyMemberID.AddUnique(MemberID);
	OnPartyUpdate.Broadcast();
}

void UAoAGameInstance::RemovePartyMember(FName MemberID)
{
	arrPartyMemberID.Remove(MemberID);
	OnPartyUpdate.Broadcast();
}

void UAoAGameInstance::RegisterPlayerCharacter(FName CharacterID, APlayerCharacter* PlayerCharacter)
{
	if (!CharacterID.IsNone() && PlayerCharacter)
	{
		SpawnCharaters.Add(CharacterID, PlayerCharacter);
	}
}

void UAoAGameInstance::UnregisterPlayerCharacter(FName CharacterID)
{
	if (!CharacterID.IsNone())
	{
		SpawnCharaters.Remove(CharacterID);
	}
}

APlayerCharacter* UAoAGameInstance::GetPlayerCharacterByID(FName CharacterID)
{
	if (SpawnCharaters.Contains(CharacterID))
	{
		return SpawnCharaters[CharacterID];
	}

	return nullptr;
}

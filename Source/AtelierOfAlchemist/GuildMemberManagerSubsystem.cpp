// Fill out your copyright notice in the Description page of Project Settings.


#include "GuildMemberManagerSubsystem.h"
#include "DataAssets/CharacterDataAsset.h"
#include "PlayerRuntimeData.h"

void UGuildMemberManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const FString DefaultCharacterDataAssetPath = TEXT("/Game/Blueprints/Data/Characters/Party/DA_Reina.DA_Reina");
	const FString DefaultCharacterDataAssetPath1 = TEXT("/Game/Blueprints/Data/Characters/Party/DA_Sato.DA_Sato");
	const FString DefaultCharacterDataAssetPath2 = TEXT("/Game/Blueprints/Data/Characters/Party/DA_Yuu.DA_Yuu");
	UCharacterDataAsset* DefaultCharacterData = LoadObject<UCharacterDataAsset>(nullptr, *DefaultCharacterDataAssetPath);
	UCharacterDataAsset* DefaultCharacterData1 = LoadObject<UCharacterDataAsset>(nullptr, *DefaultCharacterDataAssetPath1);
	UCharacterDataAsset* DefaultCharacterData2 = LoadObject<UCharacterDataAsset>(nullptr, *DefaultCharacterDataAssetPath2);

	if (DefaultCharacterData != nullptr)
	{
		AddGuildMember(DefaultCharacterData);
		AddPartyMember(DefaultMemberID);
		AddGuildMember(DefaultCharacterData1);
		AddPartyMember("DA_Sato");
		AddGuildMember(DefaultCharacterData2);
		AddPartyMember("DA_Yuu");
	}
}

void UGuildMemberManagerSubsystem::AddGuildMember(UCharacterDataAsset* CharacterData)
{
	if (CharacterData != nullptr)
	{
		FName CharacterID = CharacterData->GetFName();
		if (!GuildMemberDataMap.Contains(CharacterID))
		{
			UPlayerRuntimeData* NewData = NewObject<UPlayerRuntimeData>(this);
			NewData->Initialize(CharacterData);
			GuildMemberDataMap.Add(CharacterID, NewData);
		}
	}
}

void UGuildMemberManagerSubsystem::AddPartyMember(FName CharacterID)
{
	if (GuildMemberDataMap.Contains(CharacterID))
	{
		PartyMemberIDs.AddUnique(CharacterID);
		OnPartyUpdate.Broadcast();
	}
}

void UGuildMemberManagerSubsystem::RemovePartyMember(FName CharacterID)
{
	if (GuildMemberDataMap.Contains(CharacterID))
	{
		PartyMemberIDs.Remove(CharacterID);
		OnPartyUpdate.Broadcast();
	}
}

UPlayerRuntimeData* UGuildMemberManagerSubsystem::GetPlayerRuntimeData(FName CharacterID) const
{
	return GuildMemberDataMap.FindRef(CharacterID);
}

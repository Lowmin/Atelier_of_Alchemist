// Fill out your copyright notice in the Description page of Project Settings.


#include "GuildMemberManagerSubsystem.h"
#include "DataAssets/CharacterDataAsset.h"
#include "PlayerRuntimeData.h"

void UGuildMemberManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const FString DefaultCharacterDataAssetPath = TEXT("/Game/Blueprints/Data/Characters/Party/DA_Reina.DA_Reina");
	const FString DefaultCharacterDataAssetPath1 = TEXT("/Game/Blueprints/Data/Characters/Party/DA_Sato.DA_Sato");
	UCharacterDataAsset* DefaultCharacterData = LoadObject<UCharacterDataAsset>(nullptr, *DefaultCharacterDataAssetPath);
	UCharacterDataAsset* DefaultCharacterData1 = LoadObject<UCharacterDataAsset>(nullptr, *DefaultCharacterDataAssetPath1);

	if (DefaultCharacterData != nullptr)
	{
		AddGuildMember(DefaultCharacterData);
		AddPartyMember(DefaultMemberID);
		AddGuildMember(DefaultCharacterData1);
		AddPartyMember("DA_Sato");
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

void UGuildMemberManagerSubsystem::ApplyDamageToAllPartyMembers(float DamageAmount)
{
	if (DamageAmount <= 0.f) return;

	// 1. 현재 파티 ID 목록을 가져옵니다.
	for (const FName& CharacterID : PartyMemberIDs)
	{
		// 2. 맵에서 해당 ID의 '실시간 데이터 객체'를 찾습니다.
		UPlayerRuntimeData* RuntimeData = GetPlayerRuntimeData(CharacterID); // 이미 만들어둔 헬퍼 함수 사용

		if (RuntimeData)
		{
			// 3. 데이터 객체의 데미지 함수를 호출합니다.
			//    (이 함수가 델리게이트를 호출하여 UI를 자동으로 업데이트합니다.)
			RuntimeData->ApplyDamage(DamageAmount);
		}
	}
}

UPlayerRuntimeData* UGuildMemberManagerSubsystem::GetPlayerRuntimeData(FName CharacterID) const
{
	return GuildMemberDataMap.FindRef(CharacterID);
}

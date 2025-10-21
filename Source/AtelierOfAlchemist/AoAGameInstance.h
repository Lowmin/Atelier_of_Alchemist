// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AoAGameInstance.generated.h"

class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyUpdate);

UCLASS()
class ATELIEROFALCHEMIST_API UAoAGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	void AddGuildMember(FName CharacterID);
	void AddPartyMember(FName CharacterID);
	void RemovePartyMember(FName CharacterID);
	const TArray<FName>& GetPartyMemberIDs() const { return PartyMemberIDs; };

	UPROPERTY(BlueprintAssignable)
	FOnPartyUpdate OnPartyUpdate;

protected:
	UPROPERTY(SaveGame)
	TArray<FName> GuildMemberIDs;

	UPROPERTY(SaveGame)
	TArray<FName> PartyMemberIDs;

	UPROPERTY(SaveGame)
	FName CurrentPartyLeaderID;

	UPROPERTY(SaveGame)
	FName DefaultMemberID = "DA_Reina";
};
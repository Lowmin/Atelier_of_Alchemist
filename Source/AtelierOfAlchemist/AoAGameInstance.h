// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AoAGameInstance.generated.h"

class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyUpdate);
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UAoAGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void AddPartyMember(FName MemberID);
	void RemovePartyMember(FName MemberID);
	void RegisterPlayerCharacter(FName CharacterID, APlayerCharacter* PlayerCharacter);
	void UnregisterPlayerCharacter(FName CharacterID);
	APlayerCharacter* GetPlayerCharacterByID(FName CharacterID);

	const TArray<FName>& GetPartyMemberID() const { return arrPartyMemberID; }

	UPROPERTY(BlueprintAssignable)
	FOnPartyUpdate OnPartyUpdate;

protected:
	UPROPERTY()
	TArray<FName> arrPartyMemberID;

	UPROPERTY()
	TMap<FName, TObjectPtr<APlayerCharacter>> SpawnCharaters;
};

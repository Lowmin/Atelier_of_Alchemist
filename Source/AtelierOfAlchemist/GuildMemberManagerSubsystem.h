// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GuildMemberManagerSubsystem.generated.h"

class UCharacterDataAsset;
class UPlayerRuntimeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyUpdate);
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UGuildMemberManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void AddGuildMember(UCharacterDataAsset* CharacterData);

	void AddPartyMember(FName CharacterID);
	void RemovePartyMember(FName CharacterID);

	UFUNCTION(BlueprintCallable, Category = "Guild|Party")
	void ApplyDamageToAllPartyMembers(float DamageAmount);

	UFUNCTION(BlueprintPure, Category = "Guild")
	UPlayerRuntimeData* GetPlayerRuntimeData(FName CharacterID) const;

	UFUNCTION(BlueprintPure, Category = "Guild")
	const TArray<FName>& GetPartyMemberIDs() const { return PartyMemberIDs; };
	
	UPROPERTY(BlueprintAssignable)
	FOnPartyUpdate OnPartyUpdate;
protected:
	UPROPERTY(SaveGame)
	TMap<FName, TObjectPtr<UPlayerRuntimeData>> GuildMemberDataMap;

	UPROPERTY(SaveGame)
	TArray<FName> PartyMemberIDs;

	UPROPERTY()
	TMap<FName, TObjectPtr<UCharacterDataAsset>> CharacterDataMap;

	FName DefaultMemberID = "DA_Reina";
};

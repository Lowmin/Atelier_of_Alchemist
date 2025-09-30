// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "PartyMemberBase.generated.h"

/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API APartyMemberBase : public ACharacterBase
{
	GENERATED_BODY()
	
private:
	int32 MemberID{};

public:
	void JoinParty();
	void LeaveParty();
};

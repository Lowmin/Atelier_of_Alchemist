// Fill out your copyright notice in the Description page of Project Settings.


#include "AoAGameInstance.h"

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

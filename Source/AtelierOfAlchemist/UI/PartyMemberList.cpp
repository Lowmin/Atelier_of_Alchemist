// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyMemberList.h"
#include "../AoAGameInstance.h"
#include "PartyMemberSlot.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "../DataAssets/CharacterDataAsset.h"

void UPartyMemberList::NativeConstruct()
{
	GameInstance = GetGameInstance<UAoAGameInstance>();
	
	if (GameInstance)
	{
		GameInstance->OnPartyUpdate.AddDynamic(this, &UPartyMemberList::RefreshPartyList);
		RefreshPartyList();
	}

	Super::NativeConstruct();
}

void UPartyMemberList::NativeDestruct()
{
	if (GameInstance)
	{
		GameInstance->OnPartyUpdate.RemoveDynamic(this, &UPartyMemberList::RefreshPartyList);
	}

	Super::NativeDestruct();
}

void UPartyMemberList::BuildCharacterLookups()
{
	PlayerCharacterMap.Empty();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		APlayerCharacter* PC = Cast<APlayerCharacter>(Actor);
		if (PC && !PC->GetCharacterData()->GetFName().IsNone())
		{
			PlayerCharacterMap.Add(PC->GetCharacterData()->GetFName(), PC);
		}
	}
}

void UPartyMemberList::RefreshPartyList()
{
	PartyList->ClearChildren();

	const TArray<FName>& PartyMemberIDs = GameInstance->GetPartyMemberIDs();

	BuildCharacterLookups();

	for (const FName& CharacterID : PartyMemberIDs)
	{
		APlayerCharacter* PlayerCharacter = PlayerCharacterMap.FindRef(CharacterID);
		UPartyMemberSlot* PartyMemberSlot = CreateWidget<UPartyMemberSlot>(this, BP_PartyMemberSlot);

		UE_LOG(LogTemp, Warning, TEXT("%s UI Init."), *CharacterID.ToString());

		if (PartyMemberSlot)
		{
			PartyMemberSlot->InitializeSlot(PlayerCharacter);
			PartyList->AddChild(PartyMemberSlot);
		}
	}
}

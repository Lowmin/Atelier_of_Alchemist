// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyStatusSimple.h"
#include "../AoAGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/VerticalBox.h"

void UPartyStatusSimple::NativeConstruct()
{
	Super::NativeConstruct();

	UAoAGameInstance* GameInstance = Cast<UAoAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnPartyUpdate.AddDynamic(this, &UPartyStatusSimple::UpdatePartyList);
	}
}

void UPartyStatusSimple::UpdatePartyList()
{
	UAoAGameInstance* GameInstance = Cast<UAoAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr || PartyListContainer == nullptr) return;

	PartyListContainer->ClearChildren();

	const TArray<FName>& arrPartyMemberID = GameInstance->GetPartyMemberID();

	for (const FName& MemberID : arrPartyMemberID)
	{

	}
}

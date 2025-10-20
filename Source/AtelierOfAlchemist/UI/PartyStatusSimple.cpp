// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyStatusSimple.h"
#include "../AoAGameInstance.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/VerticalBox.h"
#include "PlayerStatusSimple.h"
#include "../DataAssets/CharacterDataAsset.h"

void UPartyStatusSimple::NativeConstruct()
{
	Super::NativeConstruct();
    UE_LOG(LogTemp, Warning, TEXT("PartyStatus is Already."))

	UAoAGameInstance* GameInstance = Cast<UAoAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
        GameInstance->OnPartyUpdate.AddDynamic(this, &UPartyStatusSimple::UpdatePartyList);
        UpdatePartyList();
    }
}

void UPartyStatusSimple::UpdatePartyList()
{
	UAoAGameInstance* GameInstance = Cast<UAoAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr || PartyListContainer == nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("PartyStatus error"))

		return;
	}

	PartyListContainer->ClearChildren();

	const TArray<FName>& arrPartyMemberID = GameInstance->GetPartyMemberID();

    for (const FName& MemberID : arrPartyMemberID)
    {
        APlayerCharacter* FindCharacter = GameInstance->GetPlayerCharacterByID(MemberID);

        if (PlayerStatusWidgetClass)
        {
			UPlayerStatusSimple* NewWidget = CreateWidget<UPlayerStatusSimple>(this, PlayerStatusWidgetClass);
			if (NewWidget)
			{
				NewWidget->InitWidget(FindCharacter);
				PartyListContainer->AddChild(NewWidget);
				UE_LOG(LogTemp, Warning, TEXT("Widget Init."))
			}
        }
    }
}

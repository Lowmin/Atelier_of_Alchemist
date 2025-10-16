// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyStatusSimple.h"
#include "../AoAGameInstance.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/VerticalBox.h"
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
    UE_LOG(LogTemp, Warning, TEXT("Update Start."))

	UAoAGameInstance* GameInstance = Cast<UAoAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr || PartyListContainer == nullptr) return;

	PartyListContainer->ClearChildren();

	const TArray<FName>& arrPartyMemberID = GameInstance->GetPartyMemberID();

    UE_LOG(LogTemp, Warning, TEXT("--- RefreshPartyList CALLED with %d members ---"), arrPartyMemberID.Num());

    for (const FName& MemberID : arrPartyMemberID)
    {
        FPrimaryAssetId AssetId("Character", MemberID);
        UCharacterDataAsset* CharData = Cast<UCharacterDataAsset>(UAssetManager::Get().GetPrimaryAssetObject(AssetId));

        if (CharData)
        {
            if (PlayerStatusWidgetClass)
            {
                UPlayerStatusSimple* NewWidget = CreateWidget<UPlayerStatusSimple>(this, PlayerStatusWidgetClass);

                NewWidget->SetStatus(CharData->CharacterImage, CharData->BaseMaxHealth, CharData->BaseMaxHealth);
                PartyListContainer->AddChild(NewWidget);
            }
        }
        else UE_LOG(LogTemp, Warning, TEXT("16415514514451"));

    }
}

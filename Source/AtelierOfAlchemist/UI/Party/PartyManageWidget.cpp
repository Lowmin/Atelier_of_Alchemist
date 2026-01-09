// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyManageWidget.h"
#include "../../GuildMemberManagerSubsystem.h"
#include "../../PlayerRuntimeData.h"
#include "../../DataAssets/CharacterDataAsset.h"

#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/HorizontalBox.h"

void UPartyManageWidget::InitPartyList()
{
	if(Box_PartyList) Box_PartyList->ClearChildren();

	UGuildMemberManagerSubsystem* GuildManager = GetGameInstance()->GetSubsystem<UGuildMemberManagerSubsystem>();
	TArray<FName> PartyIDs = GuildManager->GetPartyMemberIDs();

	for (int i = 0; i <= PartyIDs.Num(); i++)
	{
		FName MemberID = PartyIDs[i];
		UPlayerRuntimeData* MemberData = GuildManager->GetPlayerRuntimeData(MemberID);

		if (MemberData)
		{
			UPartyMemberSlot* NewSlot = CreateWidget<UPartyMemberSlot>(this, MemberSlotClass);
			
			if (NewSlot)
			{
				NewSlot->InitSlot(MemberData);
				NewSlot->OnSlotSelected.AddDynamic(this, &UPartyManageWidget::OnMemberSelected);
			}
		}
	}
}

void UPartyManageWidget::OnMemberSelected(UPlayerRuntimeData* Data)
{
	CurrentSelectedData = Data;
	UCharacterDataAsset* CharacterDataAsset = Data->GetCharacterDataAsset();

	if(Text_CharacterName) Text_CharacterName->SetText(CharacterDataAsset->CharacterName);
	if(Text_AttackPower) Text_AttackPower->SetText(FText::AsNumber(CharacterDataAsset->BaseAttackPower));
	if(Text_Defense) Text_Defense->SetText(FText::AsNumber(CharacterDataAsset->BaseDefensePower));
	if(Text_MaxHealth) Text_MaxHealth->SetText(FText::AsNumber(CharacterDataAsset->BaseMaxHealth));
	if(Text_Speed) Text_Speed->SetText(FText::AsNumber(CharacterDataAsset->BaseSpeed));
	if(Text_Level) Text_Level->SetText(FText::AsNumber(1));
}

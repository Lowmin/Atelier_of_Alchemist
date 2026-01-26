#include "PartyManageWidget.h"
#include "../../GuildMemberManagerSubsystem.h"
#include "../../PlayerRuntimeData.h"
#include "../../DataAssets/CharacterDataAsset.h"
#include "../../InventoryManagerSubsystem.h"
#include "Components/Image.h"
#include "PartyMemberSlot.h"
#include "EquipSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "../MyHUD.h"

void UPartyManageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Slot_Weapon)
	{
		Slot_Weapon->SetPartType(EEquipPart::PET_Weapon);
		Slot_Weapon->OnSlotClicked.AddDynamic(this, &UPartyManageWidget::OnEquipSlotClicked);
	}
	if (Slot_Head)
	{
		Slot_Head->SetPartType(EEquipPart::PET_Head);
		Slot_Head->OnSlotClicked.AddDynamic(this, &UPartyManageWidget::OnEquipSlotClicked);
	}
	if (Slot_Body)
	{
		Slot_Body->SetPartType(EEquipPart::PET_Body);
		Slot_Body->OnSlotClicked.AddDynamic(this, &UPartyManageWidget::OnEquipSlotClicked);
	}
	if (Slot_Shoes)
	{
		Slot_Shoes->SetPartType(EEquipPart::PET_Shoes);
		Slot_Shoes->OnSlotClicked.AddDynamic(this, &UPartyManageWidget::OnEquipSlotClicked);
	}

	InitPartyList();
}

void UPartyManageWidget::NativeDestruct()
{
	if (CurrentSelectedData)
	{
		CurrentSelectedData->OnEquipChanged.RemoveDynamic(this, &UPartyManageWidget::UpdateUI);
	}
	Super::NativeDestruct();
}

void UPartyManageWidget::InitPartyList()
{
	if (!Box_PartyList) return;
	Box_PartyList->ClearChildren();

	UGuildMemberManagerSubsystem* GuildManager = GetGameInstance()->GetSubsystem<UGuildMemberManagerSubsystem>();
	if (!GuildManager) return;

	const TArray<FName>& PartyIDs = GuildManager->GetPartyMemberIDs();

	for (int i = 0; i < PartyIDs.Num(); i++)
	{
		FName MemberID = PartyIDs[i];
		UPlayerRuntimeData* MemberData = GuildManager->GetPlayerRuntimeData(MemberID);

		if (MemberData && MemberSlotClass)
		{
			UPartyMemberSlot* NewSlot = CreateWidget<UPartyMemberSlot>(this, MemberSlotClass);
			if (NewSlot)
			{
				NewSlot->InitSlot(MemberData);
				NewSlot->OnSlotSelected.AddDynamic(this, &UPartyManageWidget::OnMemberSelected);
				Box_PartyList->AddChildToHorizontalBox(NewSlot);

				if (i == 0)
				{
					OnMemberSelected(MemberData);
				}
			}
		}
	}
}

void UPartyManageWidget::OnMemberSelected(UPlayerRuntimeData* Data)
{
	if (CurrentSelectedData)
	{
		CurrentSelectedData->OnEquipChanged.RemoveDynamic(this, &UPartyManageWidget::UpdateUI);
	}

	CurrentSelectedData = Data;

	if (CurrentSelectedData)
	{
		CurrentSelectedData->OnEquipChanged.AddDynamic(this, &UPartyManageWidget::UpdateUI);
		UpdateUI();
	}
}

void UPartyManageWidget::UpdateUI()
{
	UE_LOG(LogTemp, Error, TEXT("[PartyManageWidget] UpdateUI Called"));

	if (!CurrentSelectedData) return;

	UCharacterDataAsset* DataAsset = CurrentSelectedData->GetCharacterDataAsset();
	if (!DataAsset) return;

	if (Image_Character) Image_Character->SetBrushFromTexture(DataAsset->CharacterImage.LoadSynchronous());
	if (Text_CharacterName) Text_CharacterName->SetText(DataAsset->CharacterName);

	if (Text_AttackPower)
		Text_AttackPower->SetText(FText::AsNumber((int32)CurrentSelectedData->GetTotalAttack()));

	if (Text_Defense)
		Text_Defense->SetText(FText::AsNumber((int32)CurrentSelectedData->GetTotalDefense()));

	if (Text_Speed)
		Text_Speed->SetText(FText::AsNumber((int32)CurrentSelectedData->GetTotalSpeed()));

	if (Text_Level)
		Text_Level->SetText(FText::AsNumber(CurrentSelectedData->GetLevel()));

	if (Text_HP)
	{
		int32 CurrentHP = (int32)CurrentSelectedData->GetCurrentHealth();
		int32 MaxHP = (int32)CurrentSelectedData->GetTotalMaxHealth();

		FText HPString = FText::Format(
			FText::FromString(TEXT("{0} / {1}")),
			FText::AsNumber(CurrentHP),
			FText::AsNumber(MaxHP)
		);
		Text_HP->SetText(HPString);
	}

	auto UpdateSlotIcon = [&](UEquipSlotWidget* SlotWidget, EEquipPart Part)
		{
			if (SlotWidget)
			{
				UItemDataAsset* Item = CurrentSelectedData->GetEquipItem(Part);
				if (Item)
				{
					SlotWidget->SetItemIcon(Item->ItemIcon);
					UE_LOG(LogTemp, Error, TEXT("[PartyManageWidget] Part [%d] Item: %s"), (int32)Part, *Item->GetName());
				}
				else
				{
					SlotWidget->SetItemIcon(nullptr);
					UE_LOG(LogTemp, Error, TEXT("[PartyManageWidget] Part [%d] Item: None"), (int32)Part);
				}
			}
		};

	UpdateSlotIcon(Slot_Weapon, EEquipPart::PET_Weapon);
	UpdateSlotIcon(Slot_Head, EEquipPart::PET_Head);
	UpdateSlotIcon(Slot_Shoes, EEquipPart::PET_Shoes);
	UpdateSlotIcon(Slot_Body, EEquipPart::PET_Body);
}

void UPartyManageWidget::OnEquipSlotClicked(EEquipPart Part)
{
	if (!CurrentSelectedData)
	{
		UE_LOG(LogTemp, Error, TEXT("[PartyManageWidget] Error: CurrentSelectedData is NULL"));
		return;
	}

	FName CharID = CurrentSelectedData->GetCharacterID();
	UE_LOG(LogTemp, Error, TEXT("[PartyManageWidget] EquipSlotClicked. Part: %d, CharID: %s"), (int32)Part, *CharID.ToString());

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMyHUD* HUDManager = Cast<AMyHUD>(PC->GetHUD()))
		{
			HUDManager->OpenInventoryForSelection(Part, CharID);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[PartyManageWidget] Error: Failed to Cast MyHUD"));
		}
	}
}
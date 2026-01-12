// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "../../InventoryManagerSubsystem.h"
#include "Components/GridPanel.h"
#include "Components/Button.h"
#include "InventorySlotStruct.h"
#include "../InventorySlot.h"
#include "InventoryItemInfo.h" 
#include "Animation/WidgetAnimation.h"
#include "../../AoAPlayerController.h"
#include "GameFramework/PlayerController.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>();
	if (InventoryManager)
	{
		InventoryManager->OnInventoryUpdated.AddDynamic(this, &UInventory::UpdateInventory);
		UpdateInventory();
	}

	if (Button_Cancel)
	{
		Button_Cancel->OnClicked.AddDynamic(this, &UInventory::OnCloseClicked);
	}
}

void UInventory::NativeDestruct()
{
	if (InventoryManager)
	{
		InventoryManager->OnInventoryUpdated.RemoveDynamic(this, &UInventory::UpdateInventory);
	}

	Super::NativeDestruct();
}

void UInventory::UpdateInventory()
{
	if (!InventoryGrid || !InventoryManager) return;

	InventoryGrid->ClearChildren();

	const TArray<FInventorySlotStruct>& Slots = InventoryManager->GetInventorySlot();
	const int32 Columns = 8;

	int32 VisibleIndex = 0;

	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		const FInventorySlotStruct& SlotData = Slots[i];
		bool bIsEmpty = SlotData.ItemData.IsNull();

		if (bIsSelectionMode)
		{
			if (!bIsEmpty)
			{
				UItemDataAsset* Asset = SlotData.ItemData.LoadSynchronous();

				if (!Asset || Asset->ItemType != EItemType::EIT_Equip || Asset->Part != FilterPart)
				{
					continue;
				}
			}
		}

		UInventorySlot* NewSlotWidget = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		if (NewSlotWidget)
		{
			NewSlotWidget->UpdateSlot(SlotData, i);
			NewSlotWidget->SetOwningInventory(this);
			NewSlotWidget->OnSlotClicked.AddDynamic(this, &UInventory::OnSlotClicked);

			const int32 Row = VisibleIndex / Columns;
			const int32 Column = VisibleIndex % Columns;
			InventoryGrid->AddChildToGrid(NewSlotWidget, Row, Column);

			VisibleIndex++;
		}
	}
}

void UInventory::Show()
{
	if (bIsAnimating || IsVisible()) return;

	if (FadeInAnim)
	{
		bIsAnimating = true;
		float AnimDuration = FadeInAnim->GetEndTime();

		SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(FadeInAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { bIsAnimating = false; }, AnimDuration, false);
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UInventory::Hide()
{
	if (bIsAnimating || !IsVisible()) return;

	if (FadeOutAnim)
	{
		bIsAnimating = true;
		float AnimDuration = FadeOutAnim->GetEndTime();

		PlayAnimation(FadeOutAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,
			[this]() { SetVisibility(ESlateVisibility::Collapsed); bIsAnimating = false; },
			AnimDuration, false);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInventory::OpenAsSelectionMode(EEquipPart InPart)
{
	bIsSelectionMode = true;
	FilterPart = InPart;

	UpdateInventory();
	Show();
}

void UInventory::OnSlotClicked(int32 SlotIndex)
{
	if (bIsSelectionMode)
	{
		if (InventoryManager)
		{
			const TArray<FInventorySlotStruct>& Slots = InventoryManager->GetInventorySlot();
			if (Slots.IsValidIndex(SlotIndex) && Slots[SlotIndex].ItemData.IsNull())
			{
				return;
			}
		}

		OnItemSelected.Broadcast(SlotIndex);
		Hide();
		bIsSelectionMode = false;

		if (APlayerController* PC = GetOwningPlayer())
		{
			// 파티 메뉴가 열려있지 않을 때만 게임 모드로 복귀 (이전 로직 유지)
			// (IsPartyMenuOpen() 함수가 없으면 이 부분은 기존 코드대로 두세요)
			// PC->SetShowMouseCursor(false);
			// PC->SetInputMode(FInputModeGameOnly());

			// *주의: 파티창 연동 로직을 넣으셨다면 OnCloseClicked처럼 PC->IsPartyMenuOpen() 체크가 필요할 수 있습니다.
		}
	}
}

void UInventory::OnCloseClicked()
{
	Hide();
	bIsSelectionMode = false;

	if (AAoAPlayerController* PC = Cast<AAoAPlayerController>(GetOwningPlayer()))
	{
		if (PC->IsPartyMenuOpen())
		{
			return;
		}

		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
}
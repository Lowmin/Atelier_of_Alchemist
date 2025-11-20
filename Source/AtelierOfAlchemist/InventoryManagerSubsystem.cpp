// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerSubsystem.h"
#include "UI/Notification/NotificationData.h"
#include "UI/MyHUD.h"

void UInventoryManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const FString DefaultDataAssetPath = TEXT("/Game/Blueprints/Data/Etc/DA_Wood.DA_Wood");
	UItemDataAsset* DefaultItemDataAsset = LoadObject<UItemDataAsset>(nullptr, *DefaultDataAssetPath);

	if (InventorySlot.Num() == 0)
	{
		InventorySlot.SetNum(MaxInventorySlot);
	}
}

bool UInventoryManagerSubsystem::AddItem(const UObject* WorldContextObject, UItemDataAsset* ItemDataAsset, EItemGrade ItemGrade, int32 Amount)
{
	if (!ItemDataAsset || Amount <= 0 || !WorldContextObject) return false;

	int32 MaxStack = ItemDataAsset->MaxStackSize;
	int32 AddAmount = Amount;
	bool bAddedAny = false;

	if (MaxStack > 1)
	{
		for (int32 i = 0; i < InventorySlot.Num(); ++i)
		{
			FInventorySlotStruct& Slot = InventorySlot[i];

			if (Slot.ItemData == ItemDataAsset && Slot.Grade == ItemGrade && Slot.Quantity < MaxStack)
			{
				int32 SpaceLeft = MaxStack - Slot.Quantity;
				int32 AmountToStack = FMath::Min(SpaceLeft, AddAmount);

				Slot.Quantity += AmountToStack;
				AddAmount -= AmountToStack;
				bAddedAny = true;

				if (AddAmount == 0)
				{
					break;
				}
			}
		}
	}

	if (AddAmount > 0)
	{
		for (int32 i = 0; i < InventorySlot.Num(); ++i)
		{
			FInventorySlotStruct& Slot = InventorySlot[i];

			if (Slot.ItemData.IsNull())
			{
				int32 AmountToFill = FMath::Min(MaxStack, AddAmount);

				Slot.ItemData = ItemDataAsset;
				Slot.Grade = ItemGrade;
				Slot.Quantity = AmountToFill;

				AddAmount -= AmountToFill;
				bAddedAny = true;

				if (AddAmount == 0)
				{
					break;
				}
			}
		}
	}

	InventorySort();

	if (UWorld* World = WorldContextObject->GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			if (AMyHUD* MyHUD = Cast<AMyHUD>(PC->GetHUD()))
			{
				FNotificationData Data;

				if (bAddedAny)
				{
					FFormatNamedArguments Args;
					Args.Add(TEXT("ItemName"), ItemDataAsset->ItemName);
					Data.Message = FText::Format(FText::FromString(TEXT("{ItemName}")), Args);
					Data.Icon = ItemDataAsset->ItemIcon;
					Data.Type = ENotificationType::Item;

					if (AddAmount > 0)
					{
						// (선택) "일부만 획득했습니다" 같은 처리 가능
					}
				}
				else
				{
					Data.Message = FText::FromString(TEXT("인벤토리가 가득 찼습니다."));
					Data.Type = ENotificationType::Warning;
				}

				MyHUD->ShowNotification(Data);
			}
		}
	}

	return bAddedAny;
}

void UInventoryManagerSubsystem::InventorySort()
{
	InventorySlot.Sort([](const FInventorySlotStruct& A, const FInventorySlotStruct& B)
		{
			if (A.ItemData.IsNull()) return false;
			if (B.ItemData.IsNull()) return true;

			UItemDataAsset* ItemDataA = A.ItemData.LoadSynchronous();
			UItemDataAsset* ItemDataB = B.ItemData.LoadSynchronous();

			if (ItemDataA == nullptr) return false;
			if (ItemDataB == nullptr) return true;

			if (ItemDataA->ItemName.CompareTo(ItemDataB->ItemName) != 0)
				return ItemDataA->ItemName.CompareTo(ItemDataB->ItemName) < 0;

			if (A.Grade != B.Grade)
			return static_cast<int32>(A.Grade) < static_cast<int32>(B.Grade);

			return A.Quantity > B.Quantity;
		}
	);

	OnInventoryUpdated.Broadcast();
}

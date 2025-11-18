// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerSubsystem.h"
#include "UI/Notification/NotificationData.h"
#include "NotificationManagerSubsystem.h"

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
	bool bSuccess = false;

	if (MaxStack > 1)
	{
		for (int32 i = 0; i < InventorySlot.Num(); ++i)
		{
			// ...
			if (AddAmount == 0)
			{
				bSuccess = true;
				break;
			}
		}
	}
	if (AddAmount > 0)
	{
		for (int32 i = 0; i < InventorySlot.Num(); ++i)
		{
			// ...
			if (AddAmount == 0)
			{
				bSuccess = true;
				break;
			}
		}
	}

	InventorySort();

	UNotificationManagerSubsystem* NotifManager = nullptr;
	if (ULocalPlayer* LocalPlayer = GEngine->GetFirstLocalPlayerController(WorldContextObject->GetWorld())->GetLocalPlayer())
	{
		NotifManager = LocalPlayer->GetSubsystem<UNotificationManagerSubsystem>();
	}

	if (NotifManager)
	{
		FNotificationData Data;
		if (bSuccess)
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("ItemName"), ItemDataAsset->ItemName);
			Data.Message = FText::Format(FText::FromString(TEXT("{ItemName}")), Args);
			Data.Icon = ItemDataAsset->ItemIcon;
			Data.Type = ENotificationType::Success;
		}
		else
		{
			Data.Message = FText::FromString(TEXT("인벤토리"));
			Data.Type = ENotificationType::Warning;
		}
		NotifManager->ShowNotification(Data);
	}

	return bSuccess;
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

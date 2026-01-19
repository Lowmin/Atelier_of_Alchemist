#include "NotificationContainer.h"
#include "NotificationSlot.h"
#include "Components/VerticalBox.h"
#include "../../InventoryManagerSubsystem.h"

void UNotificationContainer::NativeConstruct()
{
	Super::NativeConstruct();

	if (UInventoryManagerSubsystem* InvMgr = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>())
	{
		InvMgr->OnItemAdded.AddDynamic(this, &UNotificationContainer::HandleItemAdded);
	}
}

void UNotificationContainer::NativeDestruct()
{
	if (UInventoryManagerSubsystem* InvMgr = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>())
	{
		if (InvMgr->OnItemAdded.IsAlreadyBound(this, &UNotificationContainer::HandleItemAdded))
		{
			InvMgr->OnItemAdded.RemoveDynamic(this, &UNotificationContainer::HandleItemAdded);
		}
	}

	Super::NativeDestruct();
}

void UNotificationContainer::HandleItemAdded(UItemDataAsset* Item, EItemGrade Grade, int32 Amount)
{
    if (!Item) return;

    FNotificationData NewData;
    NewData.Icon = Item->ItemIcon;
    NewData.Duration = 3.0f;

    FString GradeStr = TEXT("E");
    FString MessageStr = FString::Printf(TEXT("%s (%s) %d개 획득!"), *Item->ItemName.ToString(), *GradeStr, Amount);
    NewData.Message = FText::FromString(MessageStr);

    AddNotification(NewData);
}

void UNotificationContainer::AddNotification(const FNotificationData& Data)
{
    if (!NotificationSlotClass) return;

    UNotificationSlot* NewSlot = CreateWidget<UNotificationSlot>(this, NotificationSlotClass);

    if (NewSlot)
    {
        NotificationBox->AddChildToVerticalBox(NewSlot);
        NewSlot->SetNotificationData(Data);
    }
}
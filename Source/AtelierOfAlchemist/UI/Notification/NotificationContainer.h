#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationData.h" // 데이터 구조체를 위해 include
#include "../../DataAssets/ItemDataAsset.h" // 아이템 데이터 접근을 위해 include
#include "NotificationContainer.generated.h"

class UNotificationSlot;
class UVerticalBox;

UCLASS()
class ATELIEROFALCHEMIST_API UNotificationContainer : public UUserWidget
{
	GENERATED_BODY()

public:
	// 외부에서 직접 호출할 수도 있으니 유지
	void AddNotification(const FNotificationData& Data);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandleItemAdded(UItemDataAsset* Item, EItemGrade Grade, int32 Amount);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> NotificationBox;

	UPROPERTY(EditDefaultsOnly, Category = "Notification")
	TSubclassOf<UNotificationSlot> NotificationSlotClass;
};
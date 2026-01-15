// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Notification/NotificationData.h"
#include "MyHUD.generated.h"

class UMainUI;
class AActor;
class UNotificationContainer;

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	None,
	MainHUD,
	Inventory,
	PartyManage,
	Dialogue
};

UCLASS()
class ATELIEROFALCHEMIST_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OpenWidget(EWidgetType Type);

	UFUNCTION(BlueprintCallable)
	void CloseWidget(EWidgetType Type);

	UFUNCTION(BlueprintCallable)
	void CloseAllWidgets();

	UFUNCTION(BlueprintCallable)
	void ToggleWidget(EWidgetType Type);

	UUserWidget* GetWidget(EWidgetType Type);
	bool IsAnyUIMode() const { return CurrentPopupType != EWidgetType::None; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI Config")
	TMap<EWidgetType, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY()
	TMap<EWidgetType, TObjectPtr<UUserWidget>> CreatedWidgets;

	EWidgetType CurrentPopupType = EWidgetType::None;

	UUserWidget* GetOrCreateWidget(EWidgetType Type);
	void UpdateInputMode();
};

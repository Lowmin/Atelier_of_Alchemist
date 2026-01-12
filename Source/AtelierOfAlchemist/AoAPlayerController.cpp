#include "AoAPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/Playable/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/Inventory/Inventory.h"
#include "UI/MainUI.h"
#include "UI/MyHUD.h"
#include "UI/Party/PartyManageWidget.h"
#include "Animation/WidgetAnimation.h"

AAoAPlayerController::AAoAPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAoAPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AAoAPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAoAPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComonent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComonent->BindAction(IA_ToggleInventory, ETriggerEvent::Started, this, &AAoAPlayerController::ToggleInventory);

		if (IA_TogglePartyMenu)
		{
			EnhancedInputComonent->BindAction(IA_TogglePartyMenu, ETriggerEvent::Started, this, &AAoAPlayerController::TogglePartyMenu);
		}
	}
}

void AAoAPlayerController::ToggleInventory()
{
	if (PartyWidgetInstance && PartyWidgetInstance->IsInViewport()) return;

	AMyHUD* AoAMyHUD = Cast<AMyHUD>(GetHUD());
	if (!AoAMyHUD) return;

	UMainUI* AoAMainUI = AoAMyHUD->GetMainUIInstance();
	if (!AoAMainUI) return;

	UInventory* WBP_Inventory = AoAMainUI->WBP_Inventory;
	if (!WBP_Inventory) return;

	if (WBP_Inventory->IsVisible())
	{
		WBP_Inventory->Hide();
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		WBP_Inventory->Show();
		SetShowMouseCursor(true);
		SetInputMode(FInputModeGameAndUI());
	}
}

void AAoAPlayerController::TogglePartyMenu()
{
	if (PartyWidgetInstance && PartyWidgetInstance->IsInViewport())
	{
		PartyWidgetInstance->RemoveFromParent();
		PartyWidgetInstance = nullptr;

		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
		return;
	}

	AMyHUD* AoAMyHUD = Cast<AMyHUD>(GetHUD());
	if (AoAMyHUD)
	{
		UMainUI* AoAMainUI = AoAMyHUD->GetMainUIInstance();
		if (AoAMainUI && AoAMainUI->WBP_Inventory && AoAMainUI->WBP_Inventory->IsVisible())
		{
			return;
		}
	}

	if (CurrentOpenWidget && CurrentOpenWidget->IsInViewport())
	{
		return;
	}

	if (PartyWidgetClass)
	{
		if (!PartyWidgetInstance)
		{
			PartyWidgetInstance = CreateWidget<UPartyManageWidget>(this, PartyWidgetClass);
		}

		if (PartyWidgetInstance)
		{
			PartyWidgetInstance->AddToViewport();
			PartyWidgetInstance->InitPartyList();

			SetShowMouseCursor(true);
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(PartyWidgetInstance->TakeWidget());
			SetInputMode(InputMode);
		}
	}
}

void AAoAPlayerController::SetMenuState(bool bIsVisible, UUserWidget* CurrentWidget)
{
	if (bIsVisible)
	{
		FInputModeUIOnly InputMode;

		if (CurrentWidget) InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());

		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}
	else
	{
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

		bShowMouseCursor = false;
	}
}

void AAoAPlayerController::OpenObjectUI(TSubclassOf<UUserWidget> WidgetClass)
{
	if (CurrentOpenWidget)
	{
		CloseObjectUI();
	}

	if (WidgetClass)
	{
		CurrentOpenWidget = CreateWidget<UUserWidget>(this, WidgetClass);
		if (CurrentOpenWidget)
		{
			CurrentOpenWidget->AddToViewport();
			SetShowMouseCursor(true);
			SetInputMode(FInputModeGameAndUI());
		}
	}
}

void AAoAPlayerController::CloseObjectUI()
{
	if (CurrentOpenWidget)
	{
		CurrentOpenWidget->RemoveFromParent();
		CurrentOpenWidget = nullptr;
	}

	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
}

bool AAoAPlayerController::IsPartyMenuOpen() const
{
	return PartyWidgetInstance && PartyWidgetInstance->IsInViewport();
}

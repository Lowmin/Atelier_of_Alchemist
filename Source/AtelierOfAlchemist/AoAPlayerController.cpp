#include "AoAPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "UI/MyHUD.h"

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
	if (AMyHUD* HUDManager = Cast<AMyHUD>(GetHUD()))
	{
		HUDManager->ToggleWidget(EWidgetType::Inventory);
	}
}

void AAoAPlayerController::TogglePartyMenu()
{
	if (AMyHUD* HUDManager = Cast<AMyHUD>(GetHUD()))
	{
		HUDManager->ToggleWidget(EWidgetType::PartyManage);
	}
}

bool AAoAPlayerController::IsPartyMenuOpen() const
{
	if (AMyHUD* HUDManager = Cast<AMyHUD>(GetHUD()))
	{
		return HUDManager->IsAnyUIMode();
	}
	return false;
}
#include "MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	OpenWidget(EWidgetType::MainHUD);
}

UUserWidget* AMyHUD::GetOrCreateWidget(EWidgetType Type)
{
	if (CreatedWidgets.Contains(Type))
	{
		return CreatedWidgets[Type];
	}

	if (WidgetClasses.Contains(Type))
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClasses[Type]);
		if (NewWidget)
		{
			CreatedWidgets.Add(Type, NewWidget);
			return NewWidget;
		}
	}
	return nullptr;
}

void AMyHUD::OpenWidget(EWidgetType Type)
{
	UUserWidget* Widget = GetOrCreateWidget(Type);
	if (!Widget) return;

	if (!Widget->IsInViewport())
	{
		int32 ZOrder = (Type == EWidgetType::MainHUD) ? 0 : 10;
		Widget->AddToViewport(ZOrder);
	}

	if (Type != EWidgetType::MainHUD)
	{
		if (CurrentPopupType != EWidgetType::None && CurrentPopupType != Type)
		{
			CloseWidget(CurrentPopupType);
		}
		CurrentPopupType = Type;
	}

	UpdateInputMode();
}

void AMyHUD::CloseWidget(EWidgetType Type)
{
	if (CreatedWidgets.Contains(Type))
	{
		UUserWidget* Widget = CreatedWidgets[Type];
		if (Widget->IsInViewport())
		{
			Widget->RemoveFromParent();
		}
	}

	if (CurrentPopupType == Type)
	{
		CurrentPopupType = EWidgetType::None;
	}

	UpdateInputMode();
}

void AMyHUD::CloseAllWidgets()
{
	if (CurrentPopupType != EWidgetType::None)
	{
		CloseWidget(CurrentPopupType);
	}
}

void AMyHUD::ToggleWidget(EWidgetType Type)
{
	if (CurrentPopupType == Type)
		CloseWidget(Type);
	else
		OpenWidget(Type);
}

void AMyHUD::UpdateInputMode()
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	if (CurrentPopupType != EWidgetType::None)
	{
		FInputModeUIOnly InputMode;
		if (UUserWidget* Widget = GetOrCreateWidget(CurrentPopupType))
		{
			InputMode.SetWidgetToFocus(Widget->TakeWidget());
		}
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
	else
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}
}

UUserWidget* AMyHUD::GetWidget(EWidgetType Type)
{
	return GetOrCreateWidget(Type);
}

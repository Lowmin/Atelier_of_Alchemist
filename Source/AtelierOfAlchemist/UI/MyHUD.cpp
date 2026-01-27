#include "MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Inventory/Inventory.h" 

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	OpenWidget(EWidgetType::MainHUD);
}

UUserWidget* AMyHUD::GetOrCreateWidget(EWidgetType Type)
{
	if (CreatedWidgets.Contains(Type))
	{
		UUserWidget* ExistingWidget = CreatedWidgets[Type];
		if (ExistingWidget && ExistingWidget->IsValidLowLevel())
		{
			return ExistingWidget;
		}
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

int32 AMyHUD::GetWidgetZOrder(EWidgetType Type) const
{
	switch (Type)
	{
	case EWidgetType::MainHUD: return 0;
	case EWidgetType::Inventory: return 20;
	default: return 10;
	}
}

void AMyHUD::OpenWidget(EWidgetType Type)
{
	UUserWidget* Widget = GetOrCreateWidget(Type);
	if (!Widget) return;

	if (!Widget->IsInViewport())
	{
		Widget->AddToViewport(GetWidgetZOrder(Type));
	}

	if (Type != EWidgetType::MainHUD && Type != EWidgetType::Inventory)
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
		if (Widget && Widget->IsInViewport())
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
	CloseWidget(EWidgetType::Inventory);
}

void AMyHUD::ToggleWidget(EWidgetType Type)
{
	UUserWidget* Widget = GetOrCreateWidget(Type);
	if (Widget && Widget->IsInViewport())
	{
		CloseWidget(Type);
	}
	else
	{
		OpenWidget(Type);
	}
}

void AMyHUD::OpenInventoryForSelection(EEquipPart TargetPart, FName CharacterID)
{
	OpenWidget(EWidgetType::Inventory);

	if (UInventory* InvWidget = Cast<UInventory>(GetWidget(EWidgetType::Inventory)))
	{
		InvWidget->SetSelectionMode(true, TargetPart, CharacterID);
		InvWidget->RefreshInventory();
	}
	UpdateInputMode();
}

void AMyHUD::OpenInventoryForIngredient(UItemDataAsset* TargetItem)
{
	OpenWidget(EWidgetType::Inventory);

	if (UInventory* InvWidget = Cast<UInventory>(GetWidget(EWidgetType::Inventory)))
	{
		InvWidget->SetIngredientSelectionMode(TargetItem);
	}
	UpdateInputMode();
}

void AMyHUD::UpdateInputMode()
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	UUserWidget* TargetFocusWidget = nullptr;

	UUserWidget* InvWidget = CreatedWidgets.FindRef(EWidgetType::Inventory);
	if (InvWidget && InvWidget->IsInViewport())
	{
		TargetFocusWidget = InvWidget;
	}
	else
	{
		UUserWidget* RecipeWidget = CreatedWidgets.FindRef(EWidgetType::Recipe);
		if (RecipeWidget && RecipeWidget->IsInViewport())
		{
			TargetFocusWidget = RecipeWidget;
			CurrentPopupType = EWidgetType::Recipe;
		}
		else if (UUserWidget* PartyWidget = CreatedWidgets.FindRef(EWidgetType::PartyManage))
		{
			if (PartyWidget->IsInViewport())
			{
				TargetFocusWidget = PartyWidget;
				CurrentPopupType = EWidgetType::PartyManage;
			}
		}
		else if (UUserWidget* InteractionWidget = CreatedWidgets.FindRef(EWidgetType::Interaction))
		{
			if (InteractionWidget->IsInViewport())
			{
				TargetFocusWidget = InteractionWidget;
				CurrentPopupType = EWidgetType::Interaction;
			}
		}
		else if (CurrentPopupType != EWidgetType::None)
		{
			UUserWidget* PopupWidget = CreatedWidgets.FindRef(CurrentPopupType);
			if (PopupWidget && PopupWidget->IsInViewport())
			{
				TargetFocusWidget = PopupWidget;
			}
		}
	}

	if (TargetFocusWidget)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TargetFocusWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
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

bool AMyHUD::IsAnyUIMode() const
{
	UUserWidget* InvWidget = CreatedWidgets.FindRef(EWidgetType::Inventory);
	if (InvWidget && InvWidget->IsInViewport()) return true;

	if (CurrentPopupType != EWidgetType::None) return true;

	if (CreatedWidgets.Contains(EWidgetType::Recipe) && CreatedWidgets[EWidgetType::Recipe]->IsInViewport()) return true;
	if (CreatedWidgets.Contains(EWidgetType::PartyManage) && CreatedWidgets[EWidgetType::PartyManage]->IsInViewport()) return true;

	return false;
}
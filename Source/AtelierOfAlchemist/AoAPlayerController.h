#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AoAPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UMainUI;
class UUserWidget;
class UPartyManageWidget;

struct FInputActionValue;

UCLASS()
class ATELIEROFALCHEMIST_API AAoAPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAoAPlayerController();

	void SetMenuState(bool bIsVisible, UUserWidget* CurrentWidget = nullptr);
	void OpenObjectUI(TSubclassOf<UUserWidget> WidgetClass);
	void CloseObjectUI();
	bool IsPartyMenuOpen() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_ToggleInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_TogglePartyMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UMainUI> WBP_MainUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UMainUI> MainUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UPartyManageWidget> PartyWidgetClass;

	UPROPERTY()
	TObjectPtr<UPartyManageWidget> PartyWidgetInstance;

	UPROPERTY(VisibleInstanceOnly, Category = "UI")
	TObjectPtr<UUserWidget> CurrentOpenWidget;

	void ToggleInventory();
	void TogglePartyMenu();

private:
	bool bIsInventoryAnimPlay = false;
};
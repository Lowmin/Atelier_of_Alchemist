#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../DataAssets/ItemDataAsset.h" 
#include "MyHUD.generated.h"

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	None,
	MainHUD,
	Inventory,
	Status,
	Recipe,
	Interaction,
	PartyManage,
	Dialogue
};

UCLASS()
class ATELIEROFALCHEMIST_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OpenWidget(EWidgetType Type);

	UFUNCTION(BlueprintCallable)
	void CloseWidget(EWidgetType Type);

	UFUNCTION(BlueprintCallable)
	void CloseAllWidgets();

	UFUNCTION(BlueprintCallable)
	void ToggleWidget(EWidgetType Type);

	UFUNCTION(BlueprintCallable)
	class UUserWidget* GetWidget(EWidgetType Type);

	UFUNCTION(BlueprintCallable)
	void OpenInventoryForSelection(EEquipPart TargetPart, FName CharacterID);

	UFUNCTION(BlueprintCallable)
	bool IsAnyUIMode() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TMap<EWidgetType, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY()
	TMap<EWidgetType, UUserWidget*> CreatedWidgets;

	EWidgetType CurrentPopupType = EWidgetType::None;

	UUserWidget* GetOrCreateWidget(EWidgetType Type);
	void UpdateInputMode();
};
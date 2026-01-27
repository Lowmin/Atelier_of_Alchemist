#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../DataAssets/ItemDataAsset.h"
#include "Inventory.generated.h"

class UInventoryManagerSubsystem;
class UInventorySlot;
class UGridPanel;
class UInventoryItemInfo;
class UButton;
struct FInventorySlotStruct;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemSelected, int32, SelectedIndex);

UCLASS()
class ATELIEROFALCHEMIST_API UInventory : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RefreshInventory();

	void SetSelectionMode(bool bIsSelection, EEquipPart InPart, FName InCharacterID = NAME_None);
	void SetIngredientSelectionMode(UItemDataAsset* InTargetAsset);

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnInventoryItemSelected OnItemSelected;

	UInventoryItemInfo* GetItemInfoWidget() const { return ItemInfoWidget; }

	UPROPERTY(EditAnywhere, Category = "Design Preview", meta = (ClampMin = "1"))
	int32 PreviewColumns = 8;

	UPROPERTY(EditAnywhere, Category = "Design Preview", meta = (ClampMin = "1"))
	int32 PreviewRows = 4;

	UPROPERTY(EditAnywhere, Category = "Design Preview")
	FMargin SlotPadding = FMargin(4.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Design Preview")
	TSubclassOf<UInventorySlot> InventorySlotClass;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativePreConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> InventoryGrid;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Cancel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryItemInfo> ItemInfoWidget;

	UFUNCTION()
	void UpdateInventory();

	UFUNCTION()
	void OnSlotClicked(int32 SlotIndex);

	UFUNCTION()
	void OnCloseClicked();

	UFUNCTION()
	void OnItemHovered(const FInventorySlotStruct& SlotData);

	UPROPERTY()
	TObjectPtr<UInventoryManagerSubsystem> InventoryManager;

private:
	bool bIsSelectionMode = false;
	EEquipPart FilterPart = EEquipPart::None;
	FName TargetCharacterID = NAME_None;

	UPROPERTY()
	UItemDataAsset* FilterItemAsset = nullptr;
};
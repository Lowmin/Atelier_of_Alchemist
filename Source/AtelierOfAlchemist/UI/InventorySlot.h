#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../InventoryManagerSubsystem.h"
#include "InventorySlot.generated.h"

class UImage;
class UButton;
class UTextBlock;
class UInventory;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotHovered, const FInventorySlotStruct&, SlotData);

UCLASS()
class ATELIEROFALCHEMIST_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	void UpdateSlot(const FInventorySlotStruct& InSlotData, int32 InIndex);
	void InitAsUnequipSlot();

	void SetOwningInventory(UInventory* InInventory) { OwningInventory = InInventory; }

	UPROPERTY(BlueprintAssignable)
	FOnSlotClicked OnSlotClicked;

	UPROPERTY(BlueprintAssignable)
	FOnSlotHovered OnSlotHovered;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Slot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Count;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Grade;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UTexture2D> UnequipIconTexture;

	UFUNCTION()
	void OnButtonClicked();

private:
	int32 SlotIndex;
	FInventorySlotStruct SlotData;

	UPROPERTY()
	TObjectPtr<UInventory> OwningInventory;
};
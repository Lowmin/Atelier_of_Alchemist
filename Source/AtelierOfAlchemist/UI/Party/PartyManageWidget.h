#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../DataAssets/ItemDataAsset.h"
#include "PartyManageWidget.generated.h"

class UPartyMemberSlot;
class UEquipSlotWidget;
class UInventory;
class UPlayerRuntimeData;
class UTextBlock;
class UHorizontalBox;

UCLASS()
class ATELIEROFALCHEMIST_API UPartyManageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitPartyList();

	UFUNCTION()
	void UpdateUI();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPartyMemberSlot> MemberSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> Box_PartyList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlotWidget> Slot_Weapon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlotWidget> Slot_Head;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlotWidget> Slot_Body;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlotWidget> Slot_Shoes;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_CharacterName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Level;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_HP;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_AttackPower;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Defense;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Speed;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UInventory> InventoryClass;

	UPROPERTY()
	TObjectPtr<UInventory> InventoryWidget;

	UPROPERTY()
	UPlayerRuntimeData* CurrentSelectedData;

	EEquipPart PendingEquipPart;

	UFUNCTION()
	void OnMemberSelected(UPlayerRuntimeData* Data);

	UFUNCTION()
	void OnEquipSlotClicked(EEquipPart Part);

	UFUNCTION()
	void OnInventoryItemSelected(int32 InvSlotIndex);
};
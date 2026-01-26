#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../DataAssets/ItemDataAsset.h"
#include "PartyManageWidget.generated.h"

class UPlayerRuntimeData;
class UEquipSlotWidget;
class UPartyMemberSlot;
class UImage;
class UTextBlock;
class UHorizontalBox;

UCLASS()
class ATELIEROFALCHEMIST_API UPartyManageWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> Box_PartyList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Character;

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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlotWidget> Slot_Weapon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlotWidget> Slot_Head;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlotWidget> Slot_Body;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipSlotWidget> Slot_Shoes;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPartyMemberSlot> MemberSlotClass;

	UFUNCTION()
	void UpdateUI();

	UFUNCTION()
	void OnMemberSelected(UPlayerRuntimeData* Data);

	UFUNCTION()
	void OnEquipSlotClicked(EEquipPart Part);

private:
	UPROPERTY()
	TObjectPtr<UPlayerRuntimeData> CurrentSelectedData;

	void InitPartyList();
};
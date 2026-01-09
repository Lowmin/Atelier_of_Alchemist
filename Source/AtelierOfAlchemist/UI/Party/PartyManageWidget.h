// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartyMemberSlot.h"
#include "PartyManageWidget.generated.h"

class UPlayerRuntimeData;
class UTextBlock;
class UUniformGridPanel;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UPartyManageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitPartyList();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UPartyMemberSlot> MemberSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_CharacterName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_AttackPower;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Defense;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_MaxHealth;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Speed;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Level;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> Box_PartyList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Grid_SkillList;

	UFUNCTION()
	void OnMemberSelected(UPlayerRuntimeData* Data);

private:
	UPlayerRuntimeData* CurrentSelectedData;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleMainLayout.generated.h"

class UWidgetSwitcher;
class UBattleUI;
class UPlayerStatusSlot;
class UPlayerRuntimeData;
class UHorizontalBox;
class UBattleTurnWidget;
class ABattleUnit;
class USkillDataAsset;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UBattleMainLayout : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void HideBattleUI();
	void ShowBattleUI();	// 행동 선택
	void ShowSkillUI();		// 스킬 리스트
	void ShowTargetUI();	// 타겟 지정
	void InitStatusSlot(TArray<UPlayerRuntimeData*>& PartyDataList);
	void UpdateTurnSlotBar(const TArray<ABattleUnit*>& TurnQueue);
	void InitSkillList(const TArray<USkillDataAsset*>& Skills);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBattleUI> BattleUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerStatusSlot> PlayerStatusSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> StatusSlotBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBattleTurnWidget> TurnWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> SkillListContainer;
};

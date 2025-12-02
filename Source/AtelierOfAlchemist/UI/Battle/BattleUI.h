// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUI.generated.h"

class UImage;
class ABattleGameMode;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UBattleUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowUI();
	void HideUI();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Attack;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Skill;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Run;
	
	UPROPERTY()
	TObjectPtr<ABattleGameMode> BattleGameMode;
};

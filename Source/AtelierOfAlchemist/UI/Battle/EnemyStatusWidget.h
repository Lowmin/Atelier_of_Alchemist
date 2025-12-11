// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyStatusWidget.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UEnemyStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateHp(float CurrentHp, float MaxHp);
	void SetLevel(int32 Level);
	void SetName(FName Name);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText;
};

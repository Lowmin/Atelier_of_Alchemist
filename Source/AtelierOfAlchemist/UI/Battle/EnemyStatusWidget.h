// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyStatusWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UStatComponent;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UEnemyStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void UpdateHp(float CurrentHp, float MaxHp);

	void SetLevel(int32 Level);
	void SetName(FText Name);

	void BindStatComponent(UStatComponent* StatComp);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText;
};

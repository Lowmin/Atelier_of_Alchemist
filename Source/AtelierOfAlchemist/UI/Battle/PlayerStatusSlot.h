// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatusSlot.generated.h"

class UImage;
class UTextBlock;
class UProgressBar;
class UPlayerRuntimeData;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UPlayerStatusSlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HpText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Speed;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;

	UFUNCTION()
	void UpdateUI(float CurrnetHp, float MaxHp);

	virtual void NativeDestruct() override;

	void InitializeSlot(UPlayerRuntimeData* RuntimeData);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HpPercent;

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeData> CachedData;
};

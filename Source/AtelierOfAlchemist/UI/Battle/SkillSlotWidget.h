// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillSlotWidget.generated.h"

class UImage;
class UTextBlock;
class USkillDataAsset;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API USkillSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitSlot(USkillDataAsset* SkillData, int32 SlotIndex);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SkillIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SkillName;
};

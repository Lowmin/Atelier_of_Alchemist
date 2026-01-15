// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../DataAssets/SkillDataAsset.h"
#include "Blueprint/UserWidget.h"
#include "SkillSlotWidgetInStatusUI.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API USkillSlotWidgetInStatusUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_SkillIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SkillName;

	void SetSkillData(USkillDataAsset* InSkill);
};

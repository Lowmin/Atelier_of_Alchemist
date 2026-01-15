// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSlotWidget.h"
#include "../../DataAssets/SkillDataAsset.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USkillSlotWidget::InitSlot(USkillDataAsset* SkillData, int32 SlotIndex)
{
	if (SkillData)
	{
		if (SkillIcon) SkillIcon->SetBrushFromTexture(SkillData->SkillIcon);
		if (SkillName) SkillName->SetText(SkillData->SkillName);
	}
}

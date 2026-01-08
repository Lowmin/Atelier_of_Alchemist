// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSlotWidgetInStatusUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void USkillSlotWidgetInStatusUI::SetSkillData(USkillDataAsset* InSkill)
{
	if (!InSkill) return;

	Image_SkillIcon->SetBrushFromTexture(InSkill->SkillIcon);
	Text_SkillName->SetText(InSkill->SkillName);
}

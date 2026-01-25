#include "SkillSlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../../DataAssets/SkillDataAsset.h"

void USkillSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Select)
	{
		Button_Select->OnClicked.AddDynamic(this, &USkillSlotWidget::OnBtnClicked);
	}
}

void USkillSlotWidget::Setup(USkillDataAsset* SkillAsset)
{
	if (!SkillAsset) return;

	CachedSkill = SkillAsset;

	if (Text_SkillName)
	{
		Text_SkillName->SetText(SkillAsset->SkillName);
	}
}

void USkillSlotWidget::OnBtnClicked()
{
	if (CachedSkill)
	{
		OnSkillSelected.ExecuteIfBound(CachedSkill);
	}
}
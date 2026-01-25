#include "BattleMainWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "../../Characters/Battle/BattleUnit.h"
#include "../../DataAssets/SkillListComponent.h"
#include "../../DataAssets/SkillDataAsset.h"
#include "../../BattleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SkillSlotWidget.h"
#include "../../AoABattleController.h"

void UBattleMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Attack) Btn_Attack->OnClicked.AddDynamic(this, &UBattleMainWidget::OnAttackClicked);
	if (Btn_Skill) Btn_Skill->OnClicked.AddDynamic(this, &UBattleMainWidget::OnSkillClicked);
	if (Btn_Run) Btn_Run->OnClicked.AddDynamic(this, &UBattleMainWidget::OnRunClicked);

	if (SkillListScrollBox) SkillListScrollBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UBattleMainWidget::ShowMainMenu(bool bVisible)
{
	SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (bVisible && SkillListScrollBox)
	{
		SkillListScrollBox->SetVisibility(ESlateVisibility::Collapsed);
		ActionMenuPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UBattleMainWidget::ShowSkillMenu(bool bVisible)
{
	if (SkillListScrollBox)
	{
		SkillListScrollBox->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UBattleMainWidget::OnAttackClicked()
{
	AAoABattleController* PC = Cast<AAoABattleController>(GetOwningPlayer());
	if (PC)
	{
		PC->Input_Attack();
	}
}

void UBattleMainWidget::OnSkillClicked()
{
	if (SkillListScrollBox->GetVisibility() == ESlateVisibility::Visible)
	{
		ShowSkillMenu(false);
	}
	else
	{
		InitSkillList(OwnerUnit);
		ShowSkillMenu(true);
	}
}

void UBattleMainWidget::OnRunClicked()
{
	AAoABattleController* PC = Cast<AAoABattleController>(GetOwningPlayer());
	if (PC)
	{
		PC->Input_Run();
	}
}

void UBattleMainWidget::InitSkillList(ABattleUnit* Unit)
{
	if (!Unit || !SkillListScrollBox || !SkillSlotWidgetClass) return;

	OwnerUnit = Unit;
	SkillListScrollBox->ClearChildren();

	USkillListComponent* SkillComp = Unit->GetSkillComponent();
	if (!SkillComp) return;

	for (USkillDataAsset* Skill : SkillComp->GetSkillList())
	{
		if (!Skill) continue;

		USkillSlotWidget* NewSlot = CreateWidget<USkillSlotWidget>(this, SkillSlotWidgetClass);

		if (NewSlot)
		{
			NewSlot->Setup(Skill);

			NewSlot->OnSkillSelected.BindUObject(this, &UBattleMainWidget::OnSkillSelected);

			SkillListScrollBox->AddChild(NewSlot);
		}
	}
}

void UBattleMainWidget::OnSkillSelected(USkillDataAsset* SkillAsset)
{
	AAoABattleController* PC = Cast<AAoABattleController>(GetOwningPlayer());
	if (PC && SkillAsset)
	{
		PC->StartTargetingMode(SkillAsset);
	}
}
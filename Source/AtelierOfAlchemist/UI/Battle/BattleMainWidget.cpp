#include "BattleMainWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "../../Characters/Battle/BattleUnit.h"
#include "../../DataAssets/SkillListComponent.h"
#include "../../DataAssets/SkillDataAsset.h"
#include "../../BattleGameMode.h"
#include "../../AoABattleController.h"
#include "SkillSlotWidget.h"
#include "Kismet/GameplayStatics.h"

#define SCREEN_LOG(Text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Text)

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
	if (!OwnerUnit)
	{
		ABattleGameMode* GM = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());
		if (GM) InitSkillList(GM->GetCurrentUnit());
	}

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
	if (!Unit)
	{
		SCREEN_LOG(TEXT("Error: OwnerUnit is NULL!"));
		return;
	}
	if (!SkillListScrollBox)
	{
		SCREEN_LOG(TEXT("Error: ScrollBox is Not Bound!"));
		return;
	}
	if (!SkillSlotWidgetClass)
	{
		SCREEN_LOG(TEXT("Error: SkillSlotWidgetClass is NONE! Check Blueprint!"));
		return;
	}

	OwnerUnit = Unit;
	SkillListScrollBox->ClearChildren();

	USkillListComponent* SkillComp = Unit->GetSkillComponent();
	if (!SkillComp)
	{
		SCREEN_LOG(TEXT("Error: Unit has no SkillComponent!"));
		return;
	}

	const TArray<USkillDataAsset*>& Skills = SkillComp->GetSkillList();
	if (Skills.Num() == 0)
	{
		SCREEN_LOG(TEXT("Warning: Unit has 0 Skills."));
	}

	for (USkillDataAsset* Skill : Skills)
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
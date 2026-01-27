#include "BattleMainWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "Components/Border.h"
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

	if (Border_SkillList) Border_SkillList->SetVisibility(ESlateVisibility::Collapsed);
	if (SkillListScrollBox) SkillListScrollBox->SetVisibility(ESlateVisibility::Visible);
}

void UBattleMainWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsDesignTime())
	{
		if (SkillListScrollBox && SkillSlotWidgetClass)
		{
			SkillListScrollBox->ClearChildren();

			for (int32 i = 0; i < 4; ++i)
			{
				UUserWidget* Widget = CreateWidget<UUserWidget>(this, SkillSlotWidgetClass);
				if (Widget)
				{
					SkillListScrollBox->AddChild(Widget);
				}
			}

			if (Border_SkillList) Border_SkillList->SetVisibility(ESlateVisibility::Visible);
			SkillListScrollBox->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UBattleMainWidget::ShowMainMenu(bool bVisible)
{
	SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (bVisible)
	{
		if (Border_SkillList) Border_SkillList->SetVisibility(ESlateVisibility::Collapsed);
		if (ActionMenuPanel) ActionMenuPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UBattleMainWidget::ShowSkillMenu(bool bVisible)
{
	if (Border_SkillList)
	{
		Border_SkillList->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
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

	bool bIsMenuVisible = false;
	if (Border_SkillList)
	{
		bIsMenuVisible = (Border_SkillList->GetVisibility() == ESlateVisibility::Visible);
	}

	if (bIsMenuVisible)
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
	SkillListScrollBox->SetVisibility(ESlateVisibility::Visible);

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
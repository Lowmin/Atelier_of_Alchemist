#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleMainWidget.generated.h"

class UButton;
class UScrollBox;
class ABattleUnit;
class USkillDataAsset;

UCLASS()
class ATELIEROFALCHEMIST_API UBattleMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void ShowMainMenu(bool bVisible);

	void ShowSkillMenu(bool bVisible);

	void InitSkillList(ABattleUnit* Unit);

	UFUNCTION()
	void OnAttackClicked();

	UFUNCTION()
	void OnSkillClicked();

	UFUNCTION()
	void OnRunClicked();

	UFUNCTION()
	void OnSkillSelected(USkillDataAsset* SkillAsset);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPanelWidget> ActionMenuPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Attack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Skill;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Run;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> SkillListScrollBox;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SkillSlotWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<ABattleUnit> OwnerUnit;
};
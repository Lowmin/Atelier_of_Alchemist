#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleMainWidget.generated.h"

class UBorder;
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
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Battle UI")
	void ShowMainMenu(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "Battle UI")
	void ShowSkillMenu(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "Battle UI")
	void InitSkillList(ABattleUnit* Unit);

	UFUNCTION(BlueprintCallable, Category = "Battle UI")
	void OnAttackClicked();

	UFUNCTION(BlueprintCallable, Category = "Battle UI")
	void OnSkillClicked();

	UFUNCTION(BlueprintCallable, Category = "Battle UI")
	void OnRunClicked();

protected:
	UFUNCTION()
	void OnSkillSelected(USkillDataAsset* SkillAsset);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPanelWidget> ActionMenuPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Attack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_SkillList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Skill;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Run;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Battle UI")
	TObjectPtr<UScrollBox> SkillListScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle UI")
	TSubclassOf<UUserWidget> SkillSlotWidgetClass;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ABattleUnit> OwnerUnit;
};
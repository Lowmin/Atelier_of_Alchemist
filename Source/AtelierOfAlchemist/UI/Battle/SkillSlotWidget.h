#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillSlotWidget.generated.h"

class USkillDataAsset;
class UButton;
class UTextBlock;
class UImage;

DECLARE_DELEGATE_OneParam(FOnSkillSelectedDelegate, USkillDataAsset*);

UCLASS()
class ATELIEROFALCHEMIST_API USkillSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void Setup(USkillDataAsset* SkillAsset);

	FOnSkillSelectedDelegate OnSkillSelected;

	UFUNCTION()
	void OnBtnClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Select;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SkillName;

private:
	UPROPERTY()
	TObjectPtr<USkillDataAsset> CachedSkill;
};
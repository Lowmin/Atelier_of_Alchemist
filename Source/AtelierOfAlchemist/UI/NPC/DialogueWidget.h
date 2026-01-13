#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

class UTextBlock;
class UImage;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

UCLASS()
class ATELIEROFALCHEMIST_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateDialogue(const FText& Name, const TArray<FText>& Contents, UTexture2D* Portrait);

	UPROPERTY(BlueprintAssignable)
	FOnDialogueEnded OnDialogueEnded;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Portrait;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Dialogue;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Next;

private:
	UFUNCTION()
	void ShowNextDialogue();
	void DialogueEnded();

	TArray<FText> CurrentDialogues;
	int32 CurrentDialogIndex = 0;
};

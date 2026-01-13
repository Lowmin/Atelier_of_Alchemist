#include "DialogueWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../MyHUD.h"

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Next->OnClicked.RemoveDynamic(this, &UDialogueWidget::ShowNextDialogue);
	Button_Next->OnClicked.AddDynamic(this, &UDialogueWidget::ShowNextDialogue);
}

void UDialogueWidget::UpdateDialogue(const FText& Name, const TArray<FText>& Contents, UTexture2D* Portrait)
{
	CurrentDialogues = Contents;
	CurrentDialogIndex = 0;

	Image_Portrait->SetBrushFromTexture(Portrait);
	Text_Name->SetText(Name);

	ShowNextDialogue();
}

void UDialogueWidget::ShowNextDialogue()
{
	if (CurrentDialogues.IsValidIndex(CurrentDialogIndex))
	{
		Text_Dialogue->SetText(CurrentDialogues[CurrentDialogIndex]);
		CurrentDialogIndex++;
	}
	else DialogueEnded();
}

void UDialogueWidget::DialogueEnded()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(PC->GetHUD()))
		{
			MyHUD->CloseWidget(EWidgetType::Dialogue);
		}
	}
}

#include "DialogueWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../MyHUD.h"
#include "../../QuestManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Next->OnClicked.RemoveDynamic(this, &UDialogueWidget::ShowNextDialogue);
	Button_Next->OnClicked.AddDynamic(this, &UDialogueWidget::ShowNextDialogue);
}

void UDialogueWidget::UpdateDialogue(const FText& Name, const TArray<FText>& Contents, UTexture2D* Portrait, FName QuestID)
{
	CurrentDialogues = Contents;
	CurrentDialogIndex = 0;

	PendingQuestID = QuestID;

	Image_Portrait->SetBrushFromTexture(Portrait);
	Text_Name->SetText(Name);

	UE_LOG(LogTemp, Error, TEXT("1. Update Dialogue."));

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
	if (PendingQuestID != NAME_None)
	{
		UGameInstance* GameInstance = GetGameInstance();
		UQuestManagerSubsystem* QuestManagerSubsystem = GameInstance->GetSubsystem<UQuestManagerSubsystem>();

		QuestManagerSubsystem->AcceptQuest(PendingQuestID);
		PendingQuestID = NAME_None;
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(PC->GetHUD()))
		{
			MyHUD->CloseWidget(EWidgetType::Dialogue);
		}
	}
}

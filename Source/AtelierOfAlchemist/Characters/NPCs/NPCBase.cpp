#include "NPCBase.h"

#include "../../AoAPlayerController.h"
#include "../Playable/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "../../UI/NPC/DialogueWidget.h"
#include "../../UI/MyHUD.h"
#include "DialogueDataStruct.h"
#include "../../QuestManagerSubsystem.h"

ANPCBase::ANPCBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mexh"));
	Mesh->SetupAttachment(Root);

	InteractRange = CreateDefaultSubobject<USphereComponent>(TEXT("InteractRange"));
	InteractRange->SetupAttachment(Root);
	InteractRange->SetSphereRadius(200.f);

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(Root);
	InteractWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractWidget->SetVisibility(false);
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	
	InteractRange->OnComponentBeginOverlap.AddDynamic(this, &ANPCBase::OnOverlapBegin);
	InteractRange->OnComponentEndOverlap.AddDynamic(this, &ANPCBase::OnOverlapEnd);
}

void ANPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPCBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		InteractWidget->SetVisibility(true);
		Player->SetInteractObject(this);
	}
}

void ANPCBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		InteractWidget->SetVisibility(false);
		Player->SetInteractObject(nullptr);
	}
}

void ANPCBase::Interact_Implementation(APlayerCharacter* Interactor)
{
	UQuestManagerSubsystem* QuestManagerSubsystem = GetGameInstance()->GetSubsystem<UQuestManagerSubsystem>();

	if (!DialogueTable) return;

	TArray<FName> RowNames = DialogueTable->GetRowNames();

	const FDialogueData* Candidate_Quest = nullptr;
	const FDialogueData* Candidate_Normal = nullptr;

	int32 CurrnetLevel = Interactor->GetLevel();

	for (const FName& RowName : RowNames)
	{
		static const FString Context(TEXT("NPC Search Context"));
		FDialogueData* Row = DialogueTable->FindRow<FDialogueData>(RowName, Context);

		if (!Row) return;

		if (Row->DialogueType == EDialogueType::Normal) Candidate_Normal = Row;
		else
		{
			if (CurrnetLevel < Row->ReqLevel) continue;

			if (Row->QuestCondition == EQuestCondition::NotStarted)
			{
				Candidate_Quest = Row;
			}
			else if (Row->QuestCondition == EQuestCondition::InProgress)
			{
				if (QuestManagerSubsystem->GetActiveQuests().Find(Row->QuestID) && !QuestManagerSubsystem->IsQuestReadyToCompleted(Row->QuestID)) Candidate_Quest = Row;
			}
			else if (Row->QuestCondition == EQuestCondition::Completed)
			{
				if (QuestManagerSubsystem->GetActiveQuests().Find(Row->QuestID) && QuestManagerSubsystem->IsQuestReadyToCompleted(Row->QuestID))
				{
					Candidate_Quest = Row;
					break;
				}
			}
		}
	}

	const FDialogueData* SelectedRow = nullptr;

	if (Candidate_Quest) SelectedRow = Candidate_Quest;
	else SelectedRow = Candidate_Normal;

	if (SelectedRow)
	{
		AAoAPlayerController* PC = Cast<AAoAPlayerController>(Interactor->GetController());
		if (PC)
		{
			AMyHUD* MyHUD = Cast<AMyHUD>(PC->GetHUD());
			if (MyHUD)
			{
				MyHUD->OpenWidget(EWidgetType::Dialogue);
				UDialogueWidget* Widget = Cast<UDialogueWidget>(MyHUD->GetWidget(EWidgetType::Dialogue));

				if (Widget)
				{
					Widget->UpdateDialogue(
						FText::FromName(NPCName),
						SelectedRow->Dialogues,
						SelectedRow->Portrait,
						SelectedRow->QuestID
					);
				}
			}
		}
	}
}

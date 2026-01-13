#include "NPCBase.h"

#include "../../AoAPlayerController.h"
#include "../Playable/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "../../UI/NPC/DialogueWidget.h"
#include "../../UI/MyHUD.h"
#include "DialogueDataStruct.h"

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
		Player->SetInteractObject(this);
	}
}

void ANPCBase::Interact_Implementation(APlayerCharacter* Interactor)
{
	if (!DialogueTable || NPCID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("NPC Data is missing!"));
		return;
	}

	AAoAPlayerController* PC = Cast<AAoAPlayerController>(Interactor->GetController());
	if (!PC) return;

	AMyHUD* MyHUD = Cast<AMyHUD>(PC->GetHUD());
	if (!MyHUD) return;

	static const FString ContextString(TEXT("NPC Dialogue Context"));
	FNPCData* NPCData = DialogueTable->FindRow<FNPCData>(NPCID, ContextString);

	if (!NPCData) return;

	int32 CurrentLevel = Interactor->GetLevel();

	const FDialogueScenario* SelectedScenario = nullptr;
	int32 MaxConditionFound = -1;

	for (const FDialogueScenario& Scenario : NPCData->Scenarios)
	{
		if (CurrentLevel >= Scenario.MinLevelCondition && Scenario.MinLevelCondition > MaxConditionFound)
		{
			MaxConditionFound = Scenario.MinLevelCondition;
			SelectedScenario = &Scenario;
		}
	}
	if (SelectedScenario)
	{
		MyHUD->OpenWidget(EWidgetType::Dialogue);

		if (UUserWidget* Widget = MyHUD->GetWidget(EWidgetType::Dialogue))
		{
			if (UDialogueWidget* DialogueWidget = Cast<UDialogueWidget>(Widget))
			{
				DialogueWidget->UpdateDialogue(
					FText::FromName(NPCData->NPCName),
					SelectedScenario->Dialogues,
					NPCData->Portrait
				);
			}
		}
	}
}
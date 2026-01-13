#include "NPCBase.h"

#include "../../AoAPlayerController.h"
#include "../Playable/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "../../UI/NPC/DialogueWidget.h"
#include "../../UI/MyHUD.h"
#include "DialogueDataStruct.h"

// Sets default values
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

	// 2. HUD 매니저 가져오기
	AAoAPlayerController* PC = Cast<AAoAPlayerController>(Interactor->GetController());
	if (!PC) return;

	AMyHUD* MyHUD = Cast<AMyHUD>(PC->GetHUD());
	if (!MyHUD) return;

	// 3. 데이터 테이블에서 NPC 정보 찾기
	static const FString ContextString(TEXT("NPC Dialogue Context"));
	FNPCData* NPCData = DialogueTable->FindRow<FNPCData>(NPCID, ContextString);

	if (!NPCData) return;

	// 4. 플레이어 레벨에 맞는 시나리오 검색
	// (플레이어에게 GetLevel 함수가 있다고 가정. 없다면 임시로 1로 설정)
	int32 CurrentLevel = 1;
	// CurrentLevel = Interactor->GetLevel(); // 플레이어 레벨 가져오는 함수가 있다면 주석 해제

	const FDialogueScenario* SelectedScenario = nullptr;
	int32 MaxConditionFound = -1;

	for (const FDialogueScenario& Scenario : NPCData->Scenarios)
	{
		// 조건 레벨보다 내 레벨이 높거나 같고, 
		// 지금까지 찾은 것보다 더 높은 레벨의 조건이라면 갱신 (가장 적절한 대사 찾기)
		if (CurrentLevel >= Scenario.MinLevelCondition && Scenario.MinLevelCondition > MaxConditionFound)
		{
			MaxConditionFound = Scenario.MinLevelCondition;
			SelectedScenario = &Scenario;
		}
	}

	// 5. 시나리오를 찾았다면 HUD를 통해 대화창 열기
	if (SelectedScenario)
	{
		// HUD에게 대화창 열라고 지시 (Enum에 Dialogue 타입이 있어야 함)
		MyHUD->OpenWidget(EWidgetType::Dialogue);

		// 열린 위젯을 가져와서 데이터 세팅
		if (UUserWidget* Widget = MyHUD->GetWidget(EWidgetType::Dialogue))
		{
			if (UDialogueWidget* DialogueWidget = Cast<UDialogueWidget>(Widget))
			{
				// 대화 시작 함수 호출 (StartDialogueProcess는 DialogueWidget에 구현되어 있어야 함)
				DialogueWidget->UpdateDialogue(
					FText::FromName(NPCData->NPCName),
					SelectedScenario->Dialogues,
					NPCData->Portrait
				);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No suitable scenario found for Level %d"), CurrentLevel);
	}
}
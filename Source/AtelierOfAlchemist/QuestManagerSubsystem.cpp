#include "QuestManagerSubsystem.h"
#include "InventoryManagerSubsystem.h"
#include "Settings/QuestGameSettings.h"
#include "DataAssets/ItemDataAsset.h"

#include "Kismet/GameplayStatics.h"

void UQuestManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UQuestGameSettings* Settings = GetDefault<UQuestGameSettings>();

	if (Settings && !Settings->QuestDataTable.IsNull())
	{
		QuestDataTable = Settings->QuestDataTable.LoadSynchronous();
		UE_LOG(LogTemp, Warning, TEXT("데이터 테이블 로드 성공"));
	}
}

void UQuestManagerSubsystem::AcceptQuest(FName QuestID)
{
	if (QuestID.IsNone()) return;
	if (CompletedQuests.Contains(QuestID) || ActiveQuests.Contains(QuestID)) 
	{
		UE_LOG(LogTemp, Error, TEXT("이미 받거나 완료한 퀘스트 입니다."));
		return;
	}

	static const FString Context(TEXT("Quest Accept."));
	FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(QuestID, Context);
	if (!QuestData) return;

	UE_LOG(LogTemp, Error, TEXT("퀘스트 수락"));

	ActiveQuests.Add(QuestID, 0);
}

void UQuestManagerSubsystem::CompletedQuest(FName QuestID)
{
	if (QuestID.IsNone()) return;
	if (!ActiveQuests.Find(QuestID)) return;

	static const FString Context(TEXT("Completed Quest."));
	FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(QuestID, Context);

	UItemDataAsset* CompletedRewardItem = QuestData->RewardItem;
	EItemGrade CompletedRewardItemGrade = QuestData->RewardItemGrade;
	int32 CompletedRewardItemQuantity = QuestData->RewardItemQuantity;

	UInventoryManagerSubsystem* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>();
	InventoryManager->AddItem(this, CompletedRewardItem, CompletedRewardItemGrade, CompletedRewardItemQuantity);

	// 경험치 보상 추후 추가

	ActiveQuests.Remove(QuestID);
	CompletedQuests.Add(QuestID);
}

bool UQuestManagerSubsystem::IsQuestReadyToCompleted(FName QuestID)
{
	if (QuestID.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("QuestID Is None."));
		return false;
	}
	if (!ActiveQuests.Contains(QuestID))
	{
		UE_LOG(LogTemp, Error, TEXT("No ActiveQuest."));
		return false;
	}

	static const FString Context(TEXT("Check Quest Ready."));
	FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(QuestID, Context);

	if (QuestData->QuestType == EQuestType::Hunting)
	{
		int32 CurrentCount = ActiveQuests[QuestID];
		return CurrentCount >= QuestData->TargetCount;
	}

	if (QuestData->QuestType == EQuestType::Collection)
	{
		UInventoryManagerSubsystem* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>();

		if (QuestData->TargetItemAsset)
		{
			int32 CurrentItemCount = InventoryManager->GetItemCount(QuestData->TargetItemAsset);

			return CurrentItemCount >= QuestData->TargetCount;
		}
	}

	return false;
}

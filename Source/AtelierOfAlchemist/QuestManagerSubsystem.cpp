#include "QuestManagerSubsystem.h"

void UQuestManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UQuestManagerSubsystem::AcceptQuest(FName QuestID)
{
	if (QuestID == NAME_None) return;

	if (!ActiveQuests.Contains(QuestID) && !CompletedQuests.Contains(QuestID))
	{
		FActiveQuestData NewQuest;
		NewQuest.QuestID = QuestID;
		ActiveQuests.Add(QuestID, NewQuest);
	}
}

bool UQuestManagerSubsystem::IsQuestCompleted(FName QuestID) const
{
	return CompletedQuests.Contains(QuestID);
}

bool UQuestManagerSubsystem::IsQuestReadyToCompleted(FName QuestID) const
{
	if (ActiveQuests.Contains(QuestID))
	{
		return true;
	}
	return false;
}
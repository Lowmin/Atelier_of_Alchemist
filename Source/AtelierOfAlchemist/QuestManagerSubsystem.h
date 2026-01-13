#pragma once

#include "Characters/NPCs/QuestDataStruct.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestManagerSubsystem.generated.h"

UCLASS()
class ATELIEROFALCHEMIST_API UQuestManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void AcceptQuest(FName QuestID);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void CompletedQuest(FName QuestID);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TMap<FName, int32> ActiveQuests;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FName> CompletedQuests;

	UPROPERTY()
	TObjectPtr<UDataTable> QuestDataTable;
};

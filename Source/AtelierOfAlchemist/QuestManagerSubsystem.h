#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestManagerSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FActiveQuestData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStepIndex = 0;
};

UCLASS()
class ATELIEROFALCHEMIST_API UQuestManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void AcceptQuest(FName QuestID);

	UFUNCTION(BlueprintCallable)
	bool IsQuestCompleted(FName QuestID) const;

	UFUNCTION(BlueprintCallable)
	bool IsQuestReadyToCompleted(FName QuestID) const;

	UFUNCTION(BlueprintCallable)
	const TMap<FName, FActiveQuestData>& GetActiveQuests() const { return ActiveQuests; }

private:
	UPROPERTY()
	TMap<FName, FActiveQuestData> ActiveQuests;

	UPROPERTY()
	TSet<FName> CompletedQuests;
};
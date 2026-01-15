#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "QuestGameSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Quest System Settings"))
class ATELIEROFALCHEMIST_API UQuestGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Data", meta = (AllowedClasses = "DataTable"))
	TSoftObjectPtr<UDataTable> QuestDataTable;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartyMemberList.generated.h"

class UVerticalBox;
class UAoAGameInstance;
class UPartyMemberSlot;
class APlayerCharacter;

/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UPartyMemberList : public UUserWidget
{
	GENERATED_BODY()
	
public:

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void BuildCharacterLookups();

	UFUNCTION()
	void RefreshPartyList();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPartyMemberSlot> BP_PartyMemberSlot;

	UPROPERTY()
	TMap<FName, TObjectPtr<APlayerCharacter>> PlayerCharacterMap;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> PartyList;

	UPROPERTY()
	TObjectPtr<UAoAGameInstance> GameInstance;
};

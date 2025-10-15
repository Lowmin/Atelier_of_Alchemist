// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartyStatusSimple.generated.h"

class UVerticalBox;
class UPlayerStatusSimple;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UPartyStatusSimple : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> PartyListContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Party")
	TSubclassOf<UPlayerStatusSimple> PlayerStatusWidgetClass;

	UFUNCTION()
	void UpdatePartyList();
};

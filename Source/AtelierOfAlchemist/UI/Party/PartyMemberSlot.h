// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Characters/Playable/PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "PartyMemberSlot.generated.h"

class UImage;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMemberSelected, UPlayerRuntimeData*, Data);
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UPartyMemberSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitSlot(UPlayerRuntimeData* InData);

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnMemberSelected OnSlotSelected;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Portrait;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Select;

	UPROPERTY()
	UPlayerRuntimeData* LinkedData;

private:
	UFUNCTION()
	void OnButtonClicked();

};

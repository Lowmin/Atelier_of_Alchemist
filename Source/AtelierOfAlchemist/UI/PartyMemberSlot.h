// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartyMemberSlot.generated.h"

class UImage;
class UProgressBar;
class APlayerCharacter;
class UCharacterDataAsset;
class UPlayerRuntimeData;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UPartyMemberSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializeSlot(UPlayerRuntimeData* NewPlayerRuntimeData);

protected:
	UPROPERTY()
	TWeakObjectPtr<UPlayerRuntimeData> PlayerRuntimeData;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UFUNCTION()
	void OnHealthChange(float CurrentHealth, float MaxHealth);

	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly)
	float Percent{};

private:
	TWeakObjectPtr<APlayerCharacter> WidgetPlayerCharacter;
};

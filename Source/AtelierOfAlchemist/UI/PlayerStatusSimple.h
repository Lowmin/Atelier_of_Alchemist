// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatusSimple.generated.h"

class UImage;
class UTextBlock;
class UProgressBar;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UPlayerStatusSimple : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetStatus(TSoftObjectPtr<UTexture2D> CharacterImage, float CurrentHealth, float MaxHealth);
	void InitWidget(APlayerCharacter* PlayerCharacter);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> PlayerImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UFUNCTION()
	void OnHealthChangedHandler(float CurrentHealth, float MaxHealth);

private:
	float TargetPercent{};
	float CurrentPercent{};

	TWeakObjectPtr<APlayerCharacter> TargetCharacter;
};

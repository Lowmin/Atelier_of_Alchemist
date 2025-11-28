// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterDataAsset.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UCharacterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Character", GetFName());
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FText CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TSoftObjectPtr<UTexture2D> CharacterImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float BaseMaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float BaseAttackPower = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float BaseDefensePower = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float BaseSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TSoftClassPtr<APlayerCharacter> BP_Character;
};

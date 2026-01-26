#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillDataAsset.h"
#include "CharacterDataAsset.generated.h"

class ABattleUnit;

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
	FName CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FText CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TSoftObjectPtr<UTexture2D> CharacterImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	TSoftClassPtr<ABattleUnit> CharacterBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float AttackPower = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float DefensePower = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Speed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BaseLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle Info")
	TArray<TObjectPtr<USkillDataAsset>> DefaultSkills;
};
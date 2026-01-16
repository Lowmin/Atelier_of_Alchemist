// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

UENUM(BlueprintType)
enum class EItemGrade : uint8
{
	EIG_S	UMETA(DisplayName = "S"),
	EIG_A	UMETA(DisplayName = "A"),
	EIG_B	UMETA(DisplayName = "B"),
	EIG_C	UMETA(DisplayName = "C"),
	EIG_D	UMETA(DisplayName = "D"),
	EIG_E	UMETA(DisplayName = "E")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Potion	UMETA(DisplayName = "Potion"),
	EIT_Equip	UMETA(DisplayName = "Equip"),
	EIT_Ingredient	UMETA(DisplayName = "Ingredient")
};

UENUM(BlueprintType)
enum class EPotionEffectType : uint8
{
	PET_None			UMETA(DisplayName = "효과 없음"),
	PET_InstantHeal		UMETA(DisplayName = "즉시 회복"),
	PET_HealOverTime	UMETA(DisplayName = "지속 회복"),
	PET_AttackBuff		UMETA(DisplayName = "공격력 버프"),
	PET_DefenseBuff		UMETA(DisplayName = "방어력 버프")
};

UENUM(BlueprintType)
enum class EEquipPart : uint8
{
	PET_Weapon			UMETA(DisplayName = "무기"),
	PET_Head			UMETA(DisplayName = "머리"),
	PET_Body			UMETA(DisplayName = "몸"),
	PET_Shoes			UMETA(DisplayName = "신발")
};
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Item", GetFName());
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	EItemGrade ItemGrade;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 MaxStackSize = 99;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip", meta = (EditCondition = "ItemType == EItemType::EIT_Equip"))
	EEquipPart Part;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip", meta = (EditCondition = "ItemType == EItemType::EIT_Equip"))
	float EquipMaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip", meta = (EditCondition = "ItemType == EItemType::EIT_Equip"))
	float EquipAttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip", meta = (EditCondition = "ItemType == EItemType::EIT_Equip"))
	float EquipDefense;

	// 레벨 제한
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip", meta = (EditCondition = "ItemType == EItemType::EIT_Equip"))
	int32 LevelLimit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boom", meta = (EditCondition = "ItemType == EItemType::EIT_Boom"))
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion", meta = (EditCondition = "ItemType == EItemType::EIT_Potion"))
	EPotionEffectType PotionType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion", meta = (EditCondition = "ItemType == EItemType::EIT_Potion"))
	float Amount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion", meta = (EditCondition = "ItemType == EItemType::EIT_Potion"))
	float Duration;
};

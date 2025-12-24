// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillDataAsset.generated.h"

UENUM(BlueprintType)
enum class ESkillTarget : uint8
{
	Enemy		UMETA(DisplayName = "Enemy"),
	Ally		UMETA(DisplayName = "Ally"),
	Self		UMETA(DisplayName = "Self"),
};

UENUM(BlueprintType)
enum class ESkillScope : uint8
{
	Single		UMETA(DisplayName = "Single"),
	All			UMETA(DisplayName = "All")
};

UENUM(BlueprintType)
enum class ESkillEffectType : uint8
{
	Damage		UMETA(DisplayName = "Damage"),
	Heal		UMETA(DisplayName = "Heal"),
	Buff		UMETA(DisplayName = "Buff"),
	Debuff		UMETA(DisplayName = "Debuff")
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Melee       UMETA(DisplayName = "Melee"),
	Projectile  UMETA(DisplayName = "Projectile")
};

UENUM(BlueprintType)
enum class EProjectileSpawnType : uint8
{
	FromCaster	UMETA(DisplayName = "From Caster"),
	FromSky		UMETA(DisplayName = "From Sky"),
	AtLocation	UMETA(DisplayName = "At Target Location")
};

class ABattleProjectile;

UCLASS()
class ATELIEROFALCHEMIST_API USkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Skill", GetFName());
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FText SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UTexture2D> SkillIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FText SkillDescription;

	// 스킬 해금 레벨
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	int32 UnlockLevel = 0;

	// 타겟
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target")
	ESkillTarget Target;

	// 범위
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target")
	ESkillScope Scope;

	// 효과타입 (데미지, 버프, 디버프, 힐)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	ESkillEffectType EffectType;

	// 스킬타입 (근접, 투사체)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	ESkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	EProjectileSpawnType ProjectileSpawnType;

	// 투사체의 광역기 여부 (운석 등)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (EditCondition = "SkillType == ESkillType::Projectile"))
	bool IsGlobalProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (EditCondition = "SkillType == ESkillType::Projectile"))
	TSoftClassPtr<ABattleProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Power")
	float Power;
};

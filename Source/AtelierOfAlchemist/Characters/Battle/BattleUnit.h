// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "BattleUnit.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class ATELIEROFALCHEMIST_API ABattleUnit : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	ABattleUnit();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle")
	TArray<USkillDataAsset*> SkillList;

	USkillDataAsset* GetSkill(int32 Index) const;

	virtual void TurnStart();
	virtual void BattleAction_UseSkill(USkillDataAsset* Skill, const TArray<ABattleUnit*>& Targets);

	UFUNCTION(BlueprintCallable)
	void OnAnimNotify_MeleeHit();

	UFUNCTION(BlueprintCallable)
	void OnAnimNotify_ShootProjectile();

	void ApplySkillEffect(ABattleUnit* Target, USkillDataAsset* Skill);

	void SetTargetSelect(bool IsSelected);

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void ApplyDamage(ABattleUnit* Target, USkillDataAsset* Skill);
	FVector ProjectileSpawnPoint(FVector TargetPos);
	FRotator ProjectileSpawnRotation(FVector TargetPos, FVector SpawnLocation);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> HealthBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> TargetMarkerWidget;

private:
	UPROPERTY()
	USkillDataAsset* CachedCurrentSkill;

	UPROPERTY()
	TArray<ABattleUnit*> CachedTargets;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "BattleUnit.generated.h"

class UWidgetComponent;
class AAIcontroller;
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

	UFUNCTION(BlueprintCallable)
	void NotifyTurnEnd();

	void ApplySkillEffect(ABattleUnit* Target, USkillDataAsset* Skill);

	void SetTargetSelect(bool IsSelected);

	const TArray<ABattleUnit*>& GetCachedTargets() const { return CachedTargets; }

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void ApplyDamage(ABattleUnit* Target, USkillDataAsset* Skill);
	FVector ProjectileSpawnPoint(FVector TargetPos);
	FRotator ProjectileSpawnRotation(FVector TargetPos, FVector SpawnLocation);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> TargetMarkerWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

private:
	void StartAttackSequence();
	void OnAttackSequenceFinished();

private:
	UPROPERTY()
	USkillDataAsset* CachedCurrentSkill;

	UPROPERTY()
	TArray<ABattleUnit*> CachedTargets;

	UPROPERTY()
	class AAIController* AIController;

	FVector OriginalLocation;
	bool bIsReturning = false;
};
#pragma once

#include "CoreMinimal.h"
#include "../../Characters/CharacterBase.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "BattleUnit.generated.h"

class UWidgetComponent;
class AAIController;
class USkillDataAsset;
class USkillListComponent;
class ULevelSequencePlayer;
class ALevelSequenceActor;
class UBattleAIComponent;

UCLASS()
class ATELIEROFALCHEMIST_API ABattleUnit : public ACharacterBase
{
	GENERATED_BODY()

public:
	ABattleUnit();

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

	USkillListComponent* GetSkillComponent() const { return SkillComponent; }
	const TArray<ABattleUnit*>& GetCachedTargets() const { return CachedTargets; }

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void ApplyDamage(ABattleUnit* Target, USkillDataAsset* Skill);

	FVector ProjectileSpawnPoint(FVector TargetPos);
	FRotator ProjectileSpawnRotation(FVector TargetPos, FVector SpawnLocation);

	void PlaySkillCameraSequence(USkillDataAsset* Skill);

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkillListComponent> SkillComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBattleAIComponent> BattleAIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> TargetMarkerWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> SequencePlayer;

	UPROPERTY()
	TObjectPtr<ALevelSequenceActor> SequenceActor;

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
	FVector OriginalLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
	FRotator OriginalRotation;

private:
	void StartAttackSequence();
	void OnAttackSequenceFinished();
	void PreloadSkillAssets();

	UPROPERTY(Transient)
	TArray<UObject*> PreloadAssets;

	UPROPERTY()
	USkillDataAsset* CachedCurrentSkill;

	UPROPERTY()
	TArray<ABattleUnit*> CachedTargets;

	UPROPERTY()
	class AAIController* AIController;

	bool bIsReturning = false;
};
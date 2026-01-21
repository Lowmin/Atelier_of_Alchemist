#pragma once

#include "CoreMinimal.h"
#include "../../Characters/CharacterBase.h"
#include "BattleUnit.generated.h"

class UBattleAIComponent;
class UEnemyDataAsset;
class USkillListComponent;
class UStatComponent;
class UWidgetComponent;
class UPlayerRuntimeData;
class USkillDataAsset;

UENUM()
enum class EUnitActionState : uint8
{
	Idle,
	MoveToTarget,
	Attacking,
	ReturnToPos
};

UCLASS(BlueprintType, Blueprintable)
class ATELIEROFALCHEMIST_API ABattleUnit : public ACharacterBase
{
	GENERATED_BODY()

public:
	ABattleUnit();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkillListComponent> SkillComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBattleAIComponent> BattleAIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> TargetMarkerWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	void InitializeAsPlayerUnit(UPlayerRuntimeData* RuntimeData);
	void InitializeAsEnemyUnit(UEnemyDataAsset* DataAsset, int32 Level);

	void SetTargetSelected(bool bIsSelected);

	void StartAttack(ABattleUnit* Target, float FinalDamage, USkillDataAsset* SkillAsset);

	UFUNCTION(BlueprintCallable)
	void OnAttackHit();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void Die();

	USkillListComponent* GetSkillComponent() { return SkillComponent; }

protected:
	UFUNCTION()
	void OnAttackAnimationEnd(UAnimMontage* Montage, bool bInterrupted);

	void PlayAttackMontage();
	void PreloadAssetsFromSkills();

	UPROPERTY()
	TObjectPtr<ABattleUnit> PendingTarget;

	UPROPERTY()
	TObjectPtr<USkillDataAsset> CurrentSkill;

	float PendingDamage;
	FVector OriginalLocation;
	EUnitActionState ActionState;

	TArray<UObject*> PreloadedAssets;
};
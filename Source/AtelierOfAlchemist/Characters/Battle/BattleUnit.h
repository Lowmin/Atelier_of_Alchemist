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

UENUM(BlueprintType)
enum class EUnitActionState : uint8
{
	Idle,
	MoveToTarget,
	Attacking,
	ReturnToPos,
	Die
};

UCLASS(BlueprintType, Blueprintable)
class ATELIEROFALCHEMIST_API ABattleUnit : public ACharacterBase
{
	GENERATED_BODY()

public:
	ABattleUnit();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void InitializeAsPlayerUnit(UPlayerRuntimeData* RuntimeData);
	void InitializeAsEnemyUnit(UEnemyDataAsset* DataAsset, int32 Level);

	void SetTargetSelected(bool bIsSelected);
	void StartAttack(ABattleUnit* Target, float FinalDamage, USkillDataAsset* SkillAsset);
	void Die();

	void Dodge();

	UFUNCTION(BlueprintCallable)
	void OnAttackHit();

	UFUNCTION(BlueprintCallable)
	void OnDodgeBegin();

	UFUNCTION(BlueprintCallable)
	void OnDodgeEnd();

	UFUNCTION(BlueprintCallable)
	EUnitActionState GetUnitActionState() { return ActionState; }

	USkillListComponent* GetSkillComponent() { return SkillComponent; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkillListComponent> SkillComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBattleAIComponent> BattleAIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> TargetMarkerWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class UCameraComponent> UnitCamera;

protected:
	UFUNCTION()
	void OnAttackAnimationEnd(UAnimMontage* Montage, bool bInterrupted);

	void PlayAttackMontage();
	void PreloadAssetsFromSkills();

	UPROPERTY()
	TObjectPtr<ABattleUnit> PendingTarget;

	UPROPERTY()
	TObjectPtr<USkillDataAsset> CurrentSkill;

	FVector MoveDestination;
	float PendingDamage;
	FVector OriginalLocation;
	FRotator OriginalRotation;
	EUnitActionState ActionState;

	TArray<UObject*> PreloadedAssets;
	UCharacterDataAsset* CachedCharacterDataAsset;
	bool bIsDodge = false;
};
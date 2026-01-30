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

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUnitTurnFinished, ABattleUnit*, Unit);

struct FBattleContext
{
	UPROPERTY()
	ABattleUnit* Target;

	UPROPERTY()
	class USkillDataAsset* Skill;

	FVector DefaultLoc;
	FRotator DefaultRot;
};

UENUM(BlueprintType)
enum class EUnitCombatState : uint8
{
	Idle,
	Approach,
	Attacking,
	ReturnToPos,
	Dead
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

	UFUNCTION(BlueprintCallable)
	void AnimNotify_OnHit();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	float MoveSpeed;

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
	EUnitCombatState CombatState;
	void SetCombatState(EUnitCombatState NewCombatState);

	FVector DefaultLoc;
	FRotator DefaultRot;
	FVector TargetLoc;

	ABattleUnit* TargetUnit;
	float CalculateDamage(ABattleUnit* Target, USkillDataAsset* Skill);
	bool MoveToTarget(FVector TargetLoc, float DeltaTime);
};
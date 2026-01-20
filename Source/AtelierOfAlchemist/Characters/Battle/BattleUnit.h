#pragma once

#include "CoreMinimal.h"
#include "../../Characters/CharacterBase.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "BattleUnit.generated.h"

class UBattleAIComponent;
class UEnemyDataAsset;
class USkillListComponent;
class UStatComponent;
class UWidgetComponent;
class UPlayerRuntimeData;

UCLASS()
class ATELIEROFALCHEMIST_API ABattleUnit : public ACharacterBase
{
	GENERATED_BODY()

public:
	ABattleUnit();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkillListComponent> SkillComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStatComponent> StatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBattleAIComponent> BattleAIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> TargetMarkerWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	void InitializeAsPlayerUnit(UPlayerRuntimeData* RuntimeData);
	void InitializeAsEnemyUnit(UEnemyDataAsset* DataAsset, int32 Level);
	void SetTargetSelected(bool bIsSelected);

	USkillListComponent* GetSkillComponent() { return SkillComponent; }

protected:
	void PreloadAssetsFromSkills();

	TArray<UObject*> PreloadedAssets;
};
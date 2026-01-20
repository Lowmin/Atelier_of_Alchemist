#include "BattleUnit.h"
#include "Components/WidgetComponent.h"
#include "../../DataAssets/CharacterDataAsset.h"
#include "../../DataAssets/SkillListComponent.h"
#include "../../DataAssets/SkillDataAsset.h"
#include "../../DataAssets/EnemyDataAsset.h"
#include "../../Characters/StatComponent.h"
#include "../../Object/BattleProjectile.h"
#include "../Enemy/BattleAIComponent.h"

ABattleUnit::ABattleUnit()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	SkillComponent = CreateDefaultSubobject<USkillListComponent>("SkillComponent");
	BattleAIComponent = CreateDefaultSubobject<UBattleAIComponent>("BattleAIComponent");

	TargetMarkerWidget = CreateDefaultSubobject<UWidgetComponent>("TargetMarkerWidget");
	TargetMarkerWidget->SetupAttachment(RootComponent);
	TargetMarkerWidget->SetVisibility(false);
	TargetMarkerWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	TargetMarkerWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TargetMarkerWidget->SetDrawSize(FVector2D(50.0f, 50.0f));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void ABattleUnit::InitializeAsPlayerUnit(UPlayerRuntimeData* RuntimeData)
{
	if (!RuntimeData)
	{
		UE_LOG(LogTemp, Error, TEXT("RuntimeData is null: BattleUnit.cpp, InitializeAsPlayerUnit"));
		return;
	}
	if (!StatComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("StatComponent is null: BattleUnit.cpp, InitializeAsPlayerUnit"));
		return;
	}
	if (!SkillComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillComponent is null: BattleUnit.cpp, InitializeAsPlayerUnit"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Player BattleUnit Initialize Start"));

	UCharacterDataAsset* DataAsset = RuntimeData->GetCharacterDataAsset();
	StatComponent->Initialize(RuntimeData);
	SkillComponent->InitializeSkills(DataAsset->DefaultSkills, RuntimeData->GetLevel());

	PreloadAssetsFromSkills();
}

void ABattleUnit::InitializeAsEnemyUnit(UEnemyDataAsset* DataAsset, int32 Level)
{
	if (!DataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("DataAsset is null: BattleUnit.cpp, InitializeAsEnemyUnit"));
		return;
	}
	if (!StatComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("StatComponent is null: BattleUnit.cpp, InitializeAsEnemyUnit"));
		return;
	}
	if (!SkillComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillComponent is null: BattleUnit.cpp, InitializeAsEnemyUnit"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Enemy BattleUnit Initialize Start"));

	StatComponent->InitializeFromEnemy(DataAsset);
	SkillComponent->InitializeSkills(DataAsset->DefaultSkills, Level);

	PreloadAssetsFromSkills();
}

void ABattleUnit::SetTargetSelected(bool bIsSelected)
{
	if (!TargetMarkerWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("TargetMarkerWidget is null: BattleUnit.cpp, SetTargetSelected"));
		return;
	}

	TargetMarkerWidget->SetVisibility(bIsSelected);
}

void ABattleUnit::PreloadAssetsFromSkills()
{
	if (!SkillComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillComponent is null: BattleUnit.cpp, PreloadAssetsFromSkills"));
		return;
	}

	PreloadedAssets.Empty();

	const TArray<TObjectPtr<USkillDataAsset>>& MySkills = SkillComponent->GetSkillList();
	
	for (const auto& Skill : MySkills)
	{
		if (!Skill) continue;

		if (!Skill->SkillMontage.IsNull())
		{
			PreloadedAssets.Add(Skill->SkillMontage.LoadSynchronous());
		}

		if (!Skill->ProjectileClass.IsNull())
		{
			PreloadedAssets.Add(Skill->ProjectileClass.LoadSynchronous());
		}
	}
}

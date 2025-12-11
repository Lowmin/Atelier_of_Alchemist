#include "CharacterBase.h"
#include "StatComponent.h"
#include "../DataAssets/SkillDataAsset.h"
#include "Kismet/GameplayStatics.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
}

USkillDataAsset* ACharacterBase::GetSkill(int32 Index) const
{
	if (SkillList.IsValidIndex(Index))
	{
		return SkillList[Index];
	}

	return nullptr;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::BattleAction_UseSkill(USkillDataAsset* Skill, const TArray<ACharacterBase*>& Targets)
{
	if (!Skill) return;

	if (UAnimMontage* Montage = Skill->SkillAnim.LoadSynchronous())
	{
		PlayAnimMontage(Montage);
	}

	for (ACharacterBase* Target : Targets)
	{
		if (UStatComponent* TargetStat = Target->GetStatComponent())
		{
			switch (Skill->EffectType)
			{
			case ESkillEffectType::Damage:
				TargetStat->TakeDamage(Skill->Power);
				break;
			case ESkillEffectType::Heal:
				TargetStat->Heal(Skill->Power);
				break;
			}
			UE_LOG(LogTemp, Log, TEXT("%s used %s on %s"), *GetName(), *Skill->SkillName.ToString(), *Target->GetName());
		}
	}
}
void ACharacterBase::OnTurnStart()
{
	UE_LOG(LogTemp, Log, TEXT("Turn Start: %s"), *GetName());
}

int32 ACharacterBase::GetLevel() const
{
	return StatComponent ? StatComponent->GetLevel() : 1;
}

float ACharacterBase::GetCurHealth() const
{
	return StatComponent ? StatComponent->GetCurrentHealth() : 0.0f;
}

float ACharacterBase::GetMaxHealth() const
{
	return StatComponent ? StatComponent->GetMaxHealth() : 0.0f;
}

float ACharacterBase::GetAttackPower() const
{
	return StatComponent ? StatComponent->GetAttackPower() : 0.0f;
}

float ACharacterBase::GetDefense() const
{
	return StatComponent ? StatComponent->GetDefense() : 0.0f;
}

float ACharacterBase::GetSpeed() const
{
	return StatComponent ? StatComponent->GetSpeed() : 0.0f;
}

UCharacterDataAsset* ACharacterBase::GetCharacterData() const
{
	return StatComponent ? StatComponent->GetCharacterData() : nullptr;
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (StatComponent)
	{
		StatComponent->TakeDamage(DamageAmount);
	}
	return Damage;
}
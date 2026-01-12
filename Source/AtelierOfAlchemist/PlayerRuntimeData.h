#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataAssets/ItemDataAsset.h"
#include "PlayerRuntimeData.generated.h"

class UCharacterDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipChanged);

UCLASS(BlueprintType)
class ATELIEROFALCHEMIST_API UPlayerRuntimeData : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UCharacterDataAsset* NewCharacterDataAsset);
	void ApplyDamage(float dmg);

	UCharacterDataAsset* GetCharacterDataAsset() const { return CharacterDataAsset; };

	void SetCurrentHealth(float NewHp);
	float GetCurrentHealth() const { return CurrentHealth; };
	float GetMaxHealth() const { return MaxHealth; };

	int32 GetLevel() const { return Level; }
	void SetLevel(int32 NewLevel) { Level = NewLevel; }

	UPROPERTY(BlueprintReadOnly)
	FName CharacterID;

	UFUNCTION(BlueprintCallable)
	UItemDataAsset* GetEquipItem(EEquipPart Part) const;

	UFUNCTION(BlueprintCallable)
	void SetEquipItem(EEquipPart Part, UItemDataAsset* Item);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetTotalAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetTotalDefense() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetTotalMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetTotalSpeed() const;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnEquipChanged OnEquipChanged;

protected:
	UPROPERTY()
	TObjectPtr<UCharacterDataAsset> CharacterDataAsset;

	UPROPERTY()
	float CurrentHealth;

	UPROPERTY()
	float MaxHealth;

	UPROPERTY()
	int32 Level = 1;

	UPROPERTY()
	TMap<EEquipPart, UItemDataAsset*> EquippedItems;
};
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

	void SetCurrentHealth(float NewHealth);
	void SetEquipItem(EEquipPart Part, UItemDataAsset* Item);

	UItemDataAsset* GetEquipItem(EEquipPart Part) const;

	float GetTotalMaxHealth() const;
	float GetTotalAttack() const;
	float GetTotalDefense() const;
	float GetTotalSpeed() const;
	float GetCurrentHealth() const { return CurrentHealth; }
	int32 GetLevel() const { return Level; }
	UCharacterDataAsset* GetCharacterDataAsset() const { return CharacterDataAsset; }

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnEquipChanged OnEquipChanged;

protected:
	UPROPERTY()
	TObjectPtr<UCharacterDataAsset> CharacterDataAsset;

	UPROPERTY()
	TMap<EEquipPart, UItemDataAsset*> EquippedItems;

	float CurrentHealth;
	float MaxHealth;
	int32 Level;
	FName CharacterID;
};
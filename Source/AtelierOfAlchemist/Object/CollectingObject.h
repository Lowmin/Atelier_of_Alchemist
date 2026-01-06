#pragma once

#include "CoreMinimal.h"
#include "InteractableActorBase.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "../DataAssets/ItemDataAsset.h"
#include "CollectingObject.generated.h"

class UItemDataAsset;

UCLASS()
class ATELIEROFALCHEMIST_API ACollectingObject : public AInteractableActorBase
{
	GENERATED_BODY()

public:
	ACollectingObject();

	UItemDataAsset* GetItemData() const { return DroppedItemAsset; }

protected:
	virtual void BeginPlay() override;
	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;
	virtual FText GetInteractText_Implementation() const override;
	virtual TSoftObjectPtr<UTexture2D> GetInteractIcon_Implementation() const override;

	void AddCollectingItem(APlayerCharacter* Interactor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	TMap<EItemGrade, float> GradeProbability;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	TObjectPtr<UItemDataAsset> DroppedItemAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	EItemGrade MaxGrade;

	EItemGrade RandomGrade();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	ECollectingType CollectingType = ECollectingType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	bool bIsDestroy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	int32 MaxHarvestCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	int32 CurrentHarvestCount;

private:
	float AnimLength;
	FTimerHandle HarvestTimerHandle;
};
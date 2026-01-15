#include "CollectingObject.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "../DataAssets/ItemDataAsset.h"
#include "../InventoryManagerSubsystem.h"
#include "Engine/GameInstance.h"
#include "Components/SphereComponent.h"

ACollectingObject::ACollectingObject()
{
	GradeProbability.Add(EItemGrade::EIG_S, 5.0f);
	GradeProbability.Add(EItemGrade::EIG_A, 10.0f);
	GradeProbability.Add(EItemGrade::EIG_B, 15.0f);
	GradeProbability.Add(EItemGrade::EIG_C, 20.0f);
	GradeProbability.Add(EItemGrade::EIG_D, 25.0f);
	GradeProbability.Add(EItemGrade::EIG_E, 25.0f);

	MaxGrade = EItemGrade::EIG_E;

	MaxHarvestCount = 3;
	bIsDestroy = true;
}

void ACollectingObject::BeginPlay()
{
	Super::BeginPlay();

	CurrentHarvestCount = MaxHarvestCount;
}

void ACollectingObject::Interact_Implementation(APlayerCharacter* Interactor)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float CooldownDuration = 0.0f;

	if (CurrentTime - AnimLength < 0.2f) return;

	if (Interactor)
	{
		CooldownDuration = Interactor->PlayCollectingMontage(CollectingType);
	}

	if (CooldownDuration <= 0.0f) CooldownDuration = 1.0f;

	if (CurrentTime - AnimLength < CooldownDuration) return;

	if (CurrentHarvestCount <= 0) return;

	AnimLength = CurrentTime;

	GetWorld()->GetTimerManager().ClearTimer(HarvestTimerHandle);

	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &ACollectingObject::AddCollectingItem, Interactor);

	GetWorld()->GetTimerManager().SetTimer(HarvestTimerHandle, TimerDel, CooldownDuration, false);
}

FText ACollectingObject::GetInteractText_Implementation() const
{
	if (DroppedItemAsset) return DroppedItemAsset->ItemName;
	return Super::GetInteractText_Implementation();
}

TSoftObjectPtr<UTexture2D> ACollectingObject::GetInteractIcon_Implementation() const
{
	if (DroppedItemAsset) return DroppedItemAsset->ItemIcon;
	return Super::GetInteractIcon_Implementation();
}

void ACollectingObject::AddCollectingItem(APlayerCharacter* Interactor)
{
	UInventoryManagerSubsystem* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>();

	if (Interactor && InventoryManager && DroppedItemAsset)
	{
		InventoryManager->AddItem(this, DroppedItemAsset, RandomGrade(), Quantity);

		CurrentHarvestCount--;

		if (CurrentHarvestCount <= 0)
		{
			ObjectSensor->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			if (bIsDestroy)
			{
				Destroy();
			}
		}
	}
}

EItemGrade ACollectingObject::RandomGrade()
{
	float TotalProbability{};
	TArray<EItemGrade> arrGrade;
	TArray<float> arrProbability;

	int32 MaxGradeNumeric = static_cast<int32>(MaxGrade);

	for (const TPair<EItemGrade, float>& Pair : GradeProbability)
	{
		int32 CurrentGradeNumeric = static_cast<int32>(Pair.Key);

		if (CurrentGradeNumeric >= MaxGradeNumeric)
		{
			arrGrade.Add(Pair.Key);
			arrProbability.Add(Pair.Value);
			TotalProbability += Pair.Value;
		}
	}

	if (TotalProbability == 0.0f)
	{
		return MaxGrade;
	}

	float RandomRoll = FMath::FRandRange(0.0f, TotalProbability);

	for (int32 i = 0; i < arrGrade.Num(); i++)
	{
		RandomRoll -= arrProbability[i];
		if (RandomRoll <= 0.0f)
		{
			return arrGrade[i];
		}
	}
	return arrGrade.Last();
}

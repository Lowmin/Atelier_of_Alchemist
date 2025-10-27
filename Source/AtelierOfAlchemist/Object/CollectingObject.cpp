// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectingObject.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "../DataAssets/ItemDataAsset.h"
#include "../InventoryManagerSubsystem.h"
#include "Engine/GameInstance.h"

ACollectingObject::ACollectingObject()
{
	GradeProbability.Add(EItemGrade::EIG_S, 5.0f);
	GradeProbability.Add(EItemGrade::EIG_A, 10.0f);
	GradeProbability.Add(EItemGrade::EIG_B, 15.0f);
	GradeProbability.Add(EItemGrade::EIG_C, 20.0f);
	GradeProbability.Add(EItemGrade::EIG_D, 25.0f);
	GradeProbability.Add(EItemGrade::EIG_E, 25.0f);

	MaxGrade = EItemGrade::EIG_E;
}

void ACollectingObject::OnPlayerEnter_Implementation(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->SetInteractObject(this);
	}
}

void ACollectingObject::OnPlayerLeave_Implementation(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->ClearInteractObject(this);
	}
}

void ACollectingObject::Interact_Implementation(APlayerCharacter* Interactor)
{
	UInventoryManagerSubsystem* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>();

	if (Interactor && InventoryManager)
	{
		InventoryManager->AddItem(DroppedItemAsset, RandomGrade(), 1);
		Destroy();
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

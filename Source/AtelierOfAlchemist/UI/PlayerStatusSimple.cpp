// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusSimple.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "../Characters/Playable/PlayerCharacter.h"


void UPlayerStatusSimple::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->OnHealthChanged.AddDynamic(this, &UPlayerStatusSimple::OnHealthChangedHandler);

		OnHealthChangedHandler(PlayerCharacter->GetCurHealth(), PlayerCharacter->GetMaxHealth());
	}
}

void UPlayerStatusSimple::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentPercent = FMath::Lerp(CurrentPercent, TargetPercent, 0.1f);
	if (HPBar)
	{
		HPBar->SetPercent(CurrentPercent);
	}
}

void UPlayerStatusSimple::SetStatus(TSoftObjectPtr<UTexture2D> CharacterImage, float CurrentHealth, float MaxHealth)
{
	if (PlayerImage)
	{
		PlayerImage->SetBrushFromTexture(CharacterImage.LoadSynchronous());
	}

	OnHealthChangedHandler(CurrentHealth, MaxHealth);
}

void UPlayerStatusSimple::OnHealthChangedHandler(float CurrentHealth, float MaxHealth)
{
	if (MaxHealth > 0)
	{
		TargetPercent = CurrentHealth / MaxHealth;
	}
	else
	{
		TargetPercent = 0;
	}
}

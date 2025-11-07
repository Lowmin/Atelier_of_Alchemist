// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "../Object/CollectingObject.h"
#include "../DataAssets/ItemDataAsset.h"
#include "MainUI.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainUIClass)
	{
		MainUIInstance = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
		if (MainUIInstance)
		{
			MainUIInstance->AddToViewport();
		}
	}

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPawn()))
	{
		PlayerCharacter->OnInteract.AddDynamic(this, &AMyHUD::OnInteract);
	}
}

void AMyHUD::OnInteract(AActor* InteractObject)
{
	if (MainUIInstance)
	{
		if (InteractObject)
		{
			if (InteractObject->Implements<UInteractableInterface>())
			{
				FText InteractText = IInteractableInterface::Execute_GetInteractText(InteractObject);
				TSoftObjectPtr<UTexture2D> InteractIcon = IInteractableInterface::Execute_GetInteractIcon(InteractObject);
				MainUIInstance->ShowInteractWidget(InteractIcon, InteractText);
			}
		}
		else
		{
			MainUIInstance->HideInteractWidget();
		}
	}
}

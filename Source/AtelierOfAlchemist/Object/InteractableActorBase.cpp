// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActorBase.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "../AoAPlayerController.h"
#include "../UI/InteractObjectWidget.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"

AInteractableActorBase::AInteractableActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName("Object", true);

	ObjectSensor = CreateDefaultSubobject<USphereComponent>(TEXT("ObjectSensor"));
	ObjectSensor->SetupAttachment(Mesh);
	ObjectSensor->SetSphereRadius(100.0f);
}

void AInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();

	ObjectSensor->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActorBase::OnBeginOverlap);
	ObjectSensor->OnComponentEndOverlap.AddDynamic(this, &AInteractableActorBase::OnEndOverlap);
}

void AInteractableActorBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->SetInteractObject(this);

		if (PromptWidgetClass)
		{
			if (APlayerController* PC = PlayerCharacter->GetController<APlayerController>())
			{
				if (!PromptWidgetInstance)
				{
					PromptWidgetInstance = CreateWidget<UInteractObjectWidget>(PC, PromptWidgetClass);
				}

				if (PromptWidgetInstance)
				{
					PromptWidgetInstance->UpdateWidgetInfo(GetInteractIcon_Implementation(), GetInteractText_Implementation());
					PromptWidgetInstance->AddToViewport();
				}
			}
		}
	}
}

void AInteractableActorBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->ClearInteractObject(this);

		if (PromptWidgetInstance && PromptWidgetInstance->IsInViewport())
		{
			PromptWidgetInstance->RemoveFromParent();
		}
	}
}

void AInteractableActorBase::Interact_Implementation(APlayerCharacter* Interactor)
{
}

FText AInteractableActorBase::GetInteractText_Implementation() const
{
	return DefaultInteractText;
}

TSoftObjectPtr<UTexture2D> AInteractableActorBase::GetInteractIcon_Implementation() const
{
	return DefaultInteractIcon;
}
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
	PrimaryActorTick.bCanEverTick = true;
	TObjectPtr<USceneComponent> Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
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

	UE_LOG(LogTemp, Warning, TEXT("%s BeginPlay Called"), *GetName()); // BeginPlay 호출 횟수 확인

	ObjectSensor->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActorBase::OnBeginOverlap);
	ObjectSensor->OnComponentEndOverlap.AddDynamic(this, &AInteractableActorBase::OnEndOverlap);
}

void AInteractableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableActorBase::OnPlayerEnter_Implementation(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter && PromptWidgetClass)
	{
		PlayerCharacter->SetInteractObject(this);

		if (!PromptWidgetInstance)
		{
			if (APlayerController* PC = PlayerCharacter->GetController<APlayerController>())
			{
				PromptWidgetInstance = CreateWidget<UInteractObjectWidget>(PC, PromptWidgetClass);
			}
		}

		if (PromptWidgetInstance && !PromptWidgetInstance->IsInViewport())
		{
			FText Text = IInteractableInterface::Execute_GetInteractText(this);
			TSoftObjectPtr<UTexture2D> Icon = IInteractableInterface::Execute_GetInteractIcon(this);

			// 2. 프롬프트 UI에 이 정보를 전달
			PromptWidgetInstance->UpdateWidgetInfo(Icon, Text);
			PromptWidgetInstance->AddToViewport();
		}
	}
}

void AInteractableActorBase::OnPlayerLeave_Implementation(APlayerCharacter* PlayerCharacter)
{
	if (PromptWidgetInstance && PromptWidgetInstance->IsInViewport())
	{
		PromptWidgetInstance->RemoveFromParent();
	}
}

void AInteractableActorBase::Interact_Implementation(APlayerCharacter* Interactor)
{
	if (MainWidgetClass && Interactor)
	{
		if (AAoAPlayerController* PC = Cast<AAoAPlayerController>(Interactor->GetController()))
		{
			PC->OpenObjectUI(MainWidgetClass);
		}
	}
}

void AInteractableActorBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnBeginOverlap - OtherActor: %s, OtherComp: %s"), *OtherActor->GetName(), *OtherComp->GetName());

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		OnPlayerEnter(PlayerCharacter);
	}
}

void AInteractableActorBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OnEndOverlap - OtherActor: %s, OtherComp: %s"), *OtherActor->GetName(), *OtherComp->GetName());

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		OnPlayerLeave(PlayerCharacter);
	}
}

FText AInteractableActorBase::GetInteractText_Implementation() const
{
	return DefaultInteractText;
}

TSoftObjectPtr<UTexture2D> AInteractableActorBase::GetInteractIcon_Implementation() const
{
	return DefaultInteractIcon;
}


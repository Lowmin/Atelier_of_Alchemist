// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActorBase.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "../AoAPlayerController.h"
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
	// 1. 프롬프트 UI 클래스가 지정되어 있고, 플레이어가 유효한지 확인
	if (PlayerCharacter && PromptWidgetClass)
	{
		// 2. 프롬프트 UI 인스턴스가 없으면 생성
		//    (UI의 'Owner'는 PlayerController여야 함)
		if (!PromptWidgetInstance)
		{
			if (APlayerController* PC = PlayerCharacter->GetController<APlayerController>())
			{
				PromptWidgetInstance = CreateWidget<UUserWidget>(PC, PromptWidgetClass);
			}
		}

		// 3. 프롬프트 UI를 뷰포트에 추가 (보이게 함)
		if (PromptWidgetInstance && !PromptWidgetInstance->IsInViewport())
		{
			PromptWidgetInstance->AddToViewport();
		}
	}
}

/**
 * 플레이어가 범위를 벗어났을 때 -> *프롬프트 UI* 숨기기
 */
void AInteractableActorBase::OnPlayerLeave_Implementation(APlayerCharacter* PlayerCharacter)
{
	// 1. 프롬프트 UI가 뷰포트에 있다면 제거 (숨김)
	if (PromptWidgetInstance && PromptWidgetInstance->IsInViewport())
	{
		PromptWidgetInstance->RemoveFromParent();
	}
}

/**
 * 플레이어가 'F'키를 눌렀을 때 -> *메인 UI* 띄우기
 */
void AInteractableActorBase::Interact_Implementation(APlayerCharacter* Interactor)
{
	// 1. *메인 UI* 클래스가 지정되어 있고, Interactor가 유효한지 확인
	if (MainWidgetClass && Interactor)
	{
		// 2. 컨트롤러를 가져옴
		if (AAoAPlayerController* PC = Cast<AAoAPlayerController>(Interactor->GetController()))
		{
			// 3. (핵심) 컨트롤러의 범용 UI 열기 함수 호출
			PC->OpenGenericInteractUI(MainWidgetClass);
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


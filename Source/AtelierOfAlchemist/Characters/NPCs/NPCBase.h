// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Object/InteractableInterface.h"
#include "NPCBase.generated.h"

class USphereComponent;
class UWidgetComponent;

UCLASS()
class ATELIEROFALCHEMIST_API ANPCBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	ANPCBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* InteractWidget;

	// 데이터 테이블 내 ID 입력
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FName NPCID;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	UDataTable* DialogueTable;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

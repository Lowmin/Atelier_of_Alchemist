// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "InteractableActorBase.generated.h"

class USphereComponent;
class APlayerCharacter;
class UInteractObjectWidget;
class UUserWidget;

UCLASS(Abstract)
class ATELIEROFALCHEMIST_API AInteractableActorBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AInteractableActorBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> ObjectSensor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UInteractObjectWidget> PromptWidgetClass;

	UPROPERTY()
	TObjectPtr<UInteractObjectWidget> PromptWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> DefaultInteractIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FText DefaultInteractText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainWidgetClass;

	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;
	virtual FText GetInteractText_Implementation() const override;
	virtual TSoftObjectPtr<UTexture2D> GetInteractIcon_Implementation() const override;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
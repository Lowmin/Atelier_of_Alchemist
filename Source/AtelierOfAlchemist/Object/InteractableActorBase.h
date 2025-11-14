// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "InteractableActorBase.generated.h"

class USphereComponent;
class APlayerCharacter;
class UInteractObjectWidget;

UCLASS(Abstract)
class ATELIEROFALCHEMIST_API AInteractableActorBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> ObjectSensor;

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void OnPlayerEnter(APlayerCharacter* PlayerCharacter);
	virtual void OnPlayerEnter_Implementation(APlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void OnPlayerLeave(APlayerCharacter* PlayerCharacter);
	virtual void OnPlayerLeave_Implementation(APlayerCharacter* PlayerCharacter);

	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UInteractObjectWidget> PromptWidgetClass;

	UPROPERTY()
	TObjectPtr<UInteractObjectWidget> PromptWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> DefaultInteractIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FText DefaultInteractText;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual FText GetInteractText_Implementation() const override;
	virtual TSoftObjectPtr<UTexture2D> GetInteractIcon_Implementation() const override;

};

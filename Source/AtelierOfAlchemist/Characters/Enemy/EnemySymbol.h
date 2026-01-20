// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySymbol.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UEnemyPartyDataAsset;
class UEnemyDataAsset;

USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UEnemyDataAsset> EnemyData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;
};

UCLASS()
class ATELIEROFALCHEMIST_API AEnemySymbol : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySymbol();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> DetectionSphere;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleProjectile.generated.h"

class ABattleUnit;
class USkillDataAsset;
class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class ATELIEROFALCHEMIST_API ABattleProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABattleProjectile();
	void InitializeSingle(ABattleUnit* Target, USkillDataAsset* SkillData, ABattleUnit* InInstigator);
	void InitializeGlobal(const TArray<ABattleUnit*>& TargetList, USkillDataAsset* SkillData, ABattleUnit* InInstigator, FVector TargetLocation);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:	
	UPROPERTY()
	TObjectPtr<USkillDataAsset> SkillDataAsset;

	UPROPERTY()
	TObjectPtr<ABattleUnit> CastUnit;

	bool IsGlobal;

	UPROPERTY()
	TObjectPtr<ABattleUnit> SingleTarget;

	UPROPERTY()
	TArray<ABattleUnit*> GlobalTargetList;

};

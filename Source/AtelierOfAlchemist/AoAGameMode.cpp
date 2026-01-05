// Fill out your copyright notice in the Description page of Project Settings.


#include "AoAGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "AoAPlayerController.h"
#include "Engine/AssetManager.h"
#include "DataAssets/CharacterDataAsset.h"
#include "AoAGameInstance.h"

AAoAGameMode::AAoAGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/Party/Yuu/BP_Yuu.BP_Yuu_C"));

	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AAoAPlayerController::StaticClass();
}

void AAoAGameMode::BeginPlay()
{
    Super::BeginPlay();

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AoAPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/Playable/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/Inventory/Inventory.h"
#include "UI/MainUI.h"
#include "UI/MyHUD.h"
#include "Animation/WidgetAnimation.h"

AAoAPlayerController::AAoAPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAoAPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void AAoAPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAoAPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComonent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComonent->BindAction(IA_ToggleInventory, ETriggerEvent::Started, this, &AAoAPlayerController::ToggleInventory);
    }
}

void AAoAPlayerController::ToggleInventory()
{
    AMyHUD* AoAMyHUD = Cast<AMyHUD>(GetHUD());
    if (!AoAMyHUD) return;

    UMainUI* MainUI = AoAMyHUD->GetMainUIInstance();
    if (!MainUI) return;

    UInventory* WBP_Inventory = MainUI->WBP_Inventory;
    if (!WBP_Inventory) return;

    if (WBP_Inventory->IsVisible())
    {
        UWidgetAnimation* FadeOutAnim = WBP_Inventory->GetAnimation(TEXT("FadeOut_Inst"));
        if (FadeOutAnim)
        {
            WBP_Inventory->PlayAnimation(FadeOutAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);

            FTimerHandle TimerHandle;
            GetWorldTimerManager().SetTimer(TimerHandle, [WBP_Inventory]() {
                if (WBP_Inventory)
                {
                    WBP_Inventory->SetVisibility(ESlateVisibility::Collapsed);
                }
                }, FadeOutAnim->GetEndTime(), false);
        }
        else
        {
            WBP_Inventory->SetVisibility(ESlateVisibility::Collapsed);
            UE_LOG(LogTemp, Warning, TEXT("NO ANIM"));
        }

        SetShowMouseCursor(false);
        SetInputMode(FInputModeGameOnly());
    }
    else
    {
        WBP_Inventory->SetVisibility(ESlateVisibility::Visible);
        UWidgetAnimation* FadeInAnim = WBP_Inventory->GetAnimation(TEXT("FadeIn_Inst"));
        if (FadeInAnim)
        {
            WBP_Inventory->PlayAnimation(FadeInAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("NO ANIM"));
        }

        SetShowMouseCursor(true);
        SetInputMode(FInputModeGameAndUI());
    }
}

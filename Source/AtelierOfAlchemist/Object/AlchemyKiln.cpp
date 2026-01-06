// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyKiln.h"
#include "../AoAPlayerController.h"
#include "../Characters/Playable/PlayerCharacter.h"
#include "../UI/Alchemy/RecipeList.h"

void AAlchemyKiln::Interact_Implementation(APlayerCharacter* Interactor)
{
	if (!Interactor || !MainWidgetClass || !KilnRecipes) return;

	if (CachedRecipeList && CachedRecipeList->IsInViewport()) return;

	if (AAoAPlayerController* PC = Interactor->GetController<AAoAPlayerController>())
	{
		UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(PC, MainWidgetClass);

		if (URecipeList* RecipeList = Cast<URecipeList>(CreatedWidget))
		{
			CachedRecipeList = RecipeList;
			UE_LOG(LogTemp, Warning, TEXT("AddToViewport!"));
			RecipeList->InitAlchemyWindow(KilnRecipes);
			RecipeList->AddToViewport();

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(RecipeList->TakeWidget());
			PC->SetMenuState(true, RecipeList);
		}
	}
}

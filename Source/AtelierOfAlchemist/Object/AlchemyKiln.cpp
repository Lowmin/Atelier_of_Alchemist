#include "AlchemyKiln.h"
#include "../AoAPlayerController.h"
#include "../UI/MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "../UI/Alchemy/RecipeList.h"
#include "../Characters/Playable/PlayerCharacter.h"

void AAlchemyKiln::Interact_Implementation(APlayerCharacter* Interactor)
{
	if (!Interactor || !MainWidgetClass || !KilnRecipes) return;

	if (CachedRecipeList && CachedRecipeList->IsInViewport()) return;

	if (APlayerController* PC = Cast<APlayerController>(Interactor->GetController()))
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(PC->GetHUD()))
		{

			UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(PC, MainWidgetClass);
			if (URecipeList* RecipeList = Cast<URecipeList>(CreatedWidget))
			{
				CachedRecipeList = RecipeList;
				RecipeList->InitAlchemyWindow(KilnRecipes);
				RecipeList->AddToViewport(10);
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(RecipeList->TakeWidget());
				PC->SetInputMode(InputMode);
				PC->bShowMouseCursor = true;
			}
		}
	}
}

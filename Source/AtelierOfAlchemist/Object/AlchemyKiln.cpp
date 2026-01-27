#include "AlchemyKiln.h"
#include "../AoAPlayerController.h"
#include "../UI/MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "../UI/Alchemy/RecipeList.h"
#include "../Characters/Playable/PlayerCharacter.h"

void AAlchemyKiln::Interact_Implementation(APlayerCharacter* Interactor)
{
	if (!Interactor || !KilnRecipes) return;

	if (APlayerController* PC = Cast<APlayerController>(Interactor->GetController()))
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(PC->GetHUD()))
		{
			MyHUD->OpenWidget(EWidgetType::Recipe);

			if (URecipeList* RecipeList = Cast<URecipeList>(MyHUD->GetWidget(EWidgetType::Recipe)))
			{
				CachedRecipeList = RecipeList;
				RecipeList->InitAlchemyWindow(KilnRecipes);
			}
		}
	}
}

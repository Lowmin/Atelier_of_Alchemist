// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeList.h"
#include "../../AoAGameInstance.h"
#include "../../AoAPlayerController.h"
#include "../../RecipeManagerSubsystem.h"
#include "../../DataAssets/RecipeDataAsset.h"
#include "RecipeListSlot.h"
#include "Components/GridPanel.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Engine/AssetManager.h"

void URecipeList::InitAlchemyWindow(URecipeDataAsset* InRecipeData)
{
	RecipeData = InRecipeData;
    Image_SelectedIcon->SetVisibility(ESlateVisibility::Hidden);
    Text_SelectedName->SetText(FText::GetEmpty());
    Button_Craft->SetIsEnabled(false);
	RefreshRecipeList();
}

void URecipeList::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Close->OnClicked.AddDynamic(this, &URecipeList::OnCloseButtonClicked);
}

void URecipeList::RefreshRecipeList()
{
    GridPanel_RecipeList->ClearChildren();
    UAoAGameInstance* GameInst = Cast<UAoAGameInstance>(GetGameInstance());
    URecipeManagerSubsystem* RecipeManager = GameInst->GetSubsystem<URecipeManagerSubsystem>();

    for (int32 i = 0; i < TotalSlots; ++i)
    {
        URecipeListSlot* NewSlot = CreateWidget<URecipeListSlot>(this, RecipeSlotClass);
        if (!NewSlot) continue;

        int32 Row = i / MaxColumns;
        int32 Col = i % MaxColumns;

        if (RecipeData && i < RecipeData->Recipes.Num())
        {
            const FAlchemyRecipe& Recipe = RecipeData->Recipes[i];
            bool bIsKnown = RecipeManager ? RecipeManager->IsRecipeUnlocked(Recipe.RecipeID) : false;

            NewSlot->InitSlot(Recipe, bIsKnown);
            NewSlot->OnRecipeSelected.AddUObject(this, &URecipeList::HandleRecipeSelected);
        }
        else
        {
            NewSlot->InitEmpty();
        }

        UUniformGridSlot* GridSlot = GridPanel_RecipeList->AddChildToUniformGrid(NewSlot, Row, Col);

        if (GridSlot)
        {
            GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
            GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
        }
    }
}

void URecipeList::HandleRecipeSelected(const FAlchemyRecipe& InRecipe)
{
	UAoAGameInstance* GameInst = Cast<UAoAGameInstance>(GetGameInstance());
	URecipeManagerSubsystem* RecipeSubsystem = GameInst->GetSubsystem<URecipeManagerSubsystem>();

	bool bIsKnown = RecipeSubsystem->IsRecipeUnlocked(InRecipe.RecipeID);

	if (bIsKnown)
	{
        Image_SelectedIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Image_SelectedIcon->SetBrushFromTexture(InRecipe.Icon);
		Text_SelectedName->SetText(InRecipe.RecipeName);
        Button_Craft->SetIsEnabled(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown Recipe!"));
	}
}

void URecipeList::OnCloseButtonClicked()
{
	RemoveFromParent();

	if (AAoAPlayerController* PlayerController = Cast<AAoAPlayerController>(GetOwningPlayer()))
	{
		PlayerController->SetMenuState(false);
	}
}
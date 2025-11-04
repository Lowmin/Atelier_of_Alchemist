// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeList.h"
#include "../../RecipeManagerSubsystem.h"
#include "Components/GridPanel.h"

void URecipeList::NativeConstruct()
{
	Super::NativeConstruct();

	RecipeManager = GetGameInstance()->GetSubsystem<URecipeManagerSubsystem>();
	if (RecipeManager)
	{
		UpdateRecipeList();
	}
}

void URecipeList::NativeDestruct()
{
	Super::NativeDestruct();
}

void URecipeList::UpdateRecipeList()
{
	RecipeGrid->ClearChildren();
}

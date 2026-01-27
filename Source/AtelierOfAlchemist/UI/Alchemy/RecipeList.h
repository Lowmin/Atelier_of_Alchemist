#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../DataAssets/RecipeDataAsset.h"
#include "../../DataAssets/ItemDataAsset.h"
#include "RecipeList.generated.h"

class UScrollBox;
class UHorizontalBox;
class UButton;
class UTextBlock;
class UImage;
class URecipeListSlot;
class UIngredientSlot;

UCLASS()
class ATELIEROFALCHEMIST_API URecipeList : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitAlchemyWindow(URecipeDataAsset* InRecipeData);

protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_RecipeList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> Box_IngredientSlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Craft;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Close;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_SelectedIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SelectedName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_ResultGrade;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> RecipeSlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UIngredientSlot> IngredientSlotClass;

	UPROPERTY(EditAnywhere, Category = "Design")
	int32 PreviewItemCount = 5;

	UPROPERTY(EditAnywhere, Category = "Design")
	int32 PreviewIngredientCount = 3;

	UFUNCTION()
	void OnCloseButtonClicked();

	UFUNCTION()
	void OnCraftButtonClicked();

	void RefreshRecipeList();
	void HandleRecipeSelected(const FAlchemyRecipe& InRecipe);
	void CreateIngredientSlots(const FAlchemyRecipe& Recipe);
	void UpdateCraftingState();

	UFUNCTION()
	void OnRequireSlotClicked(UIngredientSlot* SlotWidget);

	UFUNCTION()
	void OnIngredientPickedFromInventory(int32 SlotIndex);

private:
	UPROPERTY()
	URecipeDataAsset* RecipeData;

	FAlchemyRecipe SelectedRecipe;

	UPROPERTY()
	TArray<UIngredientSlot*> CreatedSlots;

	UPROPERTY()
	UIngredientSlot* CurrentEditingSlot;
};
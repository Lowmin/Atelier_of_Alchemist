#include "IngredientSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UIngredientSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Select)
	{
		Button_Select->OnClicked.AddDynamic(this, &UIngredientSlot::OnButtonClicked);
	}
}

void UIngredientSlot::InitIngredientSlot(int32 InIndex, EItemGrade InGrade, int32 InQuantity, UItemDataAsset* InAsset)
{
	InventoryIndex = InIndex;
	Grade = InGrade;
	bIsSelected = true;

	if (Text_Quantity)
	{
		Text_Quantity->SetText(FText::AsNumber(InQuantity));
		Text_Quantity->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (Text_Grade)
	{
		FString GradeStr = UEnum::GetDisplayValueAsText(InGrade).ToString();
		Text_Grade->SetText(FText::FromString(GradeStr));
		Text_Grade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (Image_Icon && InAsset)
	{
		Image_Icon->SetBrushFromTexture(InAsset->ItemIcon.LoadSynchronous());
	}
}

void UIngredientSlot::InitRequirement(UItemDataAsset* InAsset, int32 InCount)
{
	RequiredAsset = InAsset;
	RequiredCount = InCount;
	bIsSelected = false;
	InventoryIndex = -1;

	if (Text_Quantity)
	{
		Text_Quantity->SetText(FText::Format(FText::FromString(TEXT("x{0}")), InCount));
		Text_Quantity->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (Text_Grade)
	{
		Text_Grade->SetText(FText::GetEmpty());
		Text_Grade->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Image_Icon && InAsset)
	{
		Image_Icon->SetBrushFromTexture(InAsset->ItemIcon.LoadSynchronous());
	}
}

void UIngredientSlot::SetSelectedMaterial(int32 InIndex, EItemGrade InGrade)
{
	InventoryIndex = InIndex;
	Grade = InGrade;
	bIsSelected = true;

	if (Text_Grade)
	{
		FString GradeStr = UEnum::GetDisplayValueAsText(InGrade).ToString();
		Text_Grade->SetText(FText::FromString(GradeStr));
		Text_Grade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UIngredientSlot::OnButtonClicked()
{
	OnRequestPopup.Broadcast(this);
}
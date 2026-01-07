#include "IngredientSlot.h"
#include "../../DataAssets/ItemDataAsset.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

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

	if (InAsset)
	{
		UTexture2D* Texture = InAsset->ItemIcon.LoadSynchronous();
		if (Texture && Image_Icon)
		{
			Image_Icon->SetBrushFromTexture(Texture);
		}
	}

	if (Text_Grade)
	{
		FString GradeStr = UEnum::GetDisplayValueAsText(Grade).ToString();
		Text_Grade->SetText(FText::FromString(GradeStr));
	}

	if (Text_Quantity)
	{
		Text_Quantity->SetText(FText::AsNumber(InQuantity));
	}
}

void UIngredientSlot::InitRequirement(UItemDataAsset* InAsset, int32 InCount)
{
	RequiredAsset = InAsset;
	RequiredCount = InCount;
	bIsSelected = false;
	InventoryIndex = -1;
	Grade = EItemGrade::EIG_E;

	if (InAsset && Image_Icon)
	{
		UTexture2D* Texture = InAsset->ItemIcon.LoadSynchronous();
		if (Texture) Image_Icon->SetBrushFromTexture(Texture);
		Image_Icon->SetColorAndOpacity(FLinearColor(0.2f, 0.2f, 0.2f, 1.0f));
	}

	if (Text_Quantity) Text_Quantity->SetText(FText::AsNumber(InCount));
	if (Text_Grade) Text_Grade->SetText(FText::GetEmpty());
}

void UIngredientSlot::SetSelectedMaterial(int32 InIndex, EItemGrade InGrade)
{
	InventoryIndex = InIndex;
	Grade = InGrade;
	bIsSelected = true;

	if (Image_Icon) Image_Icon->SetColorAndOpacity(FLinearColor::White);

	if (Text_Grade)
	{
		FString GradeStr = UEnum::GetDisplayValueAsText(Grade).ToString();
		Text_Grade->SetText(FText::FromString(GradeStr));
	}
}

void UIngredientSlot::OnButtonClicked()
{
	// 로그 추가
	UE_LOG(LogTemp, Warning, TEXT("[Slot] Clicked! IsSelected: %d"), bIsSelected);

	// 1. 팝업 창 안에서 눌렸을 때 (아이템 선택)
	if (bIsSelected)
	{
		if (OnSlotSelected.IsBound())
		{
			UE_LOG(LogTemp, Warning, TEXT("[Slot] Broadcasting Selection -> Index: %d, Grade: %d"), InventoryIndex, (int32)Grade);
			OnSlotSelected.Broadcast(InventoryIndex, Grade);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[Slot] Error: OnSlotSelected is NOT bound!"));
		}
	}

	// 2. 레시피 리스트에서 눌렸을 때 (팝업 열기 요청)
	if (OnRequestPopup.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Slot] Requesting Popup..."));
		OnRequestPopup.Broadcast(this);
	}
}
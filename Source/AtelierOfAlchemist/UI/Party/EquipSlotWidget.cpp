#include "EquipSlotWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UEquipSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Root)
	{
		Button_Root->OnClicked.AddDynamic(this, &UEquipSlotWidget::OnRootClicked);
	}
}

void UEquipSlotWidget::SetPartType(EEquipPart InPart)
{
	PartType = InPart;
}

void UEquipSlotWidget::SetItemIcon(TSoftObjectPtr<UTexture2D> Icon)
{
	if (!Image_Icon) return;

	if (Icon.IsNull())
	{
		Image_Icon->SetColorAndOpacity(FLinearColor::Transparent);
	}
	else
	{
		Image_Icon->SetColorAndOpacity(FLinearColor::White);
		Image_Icon->SetBrushFromTexture(Icon.LoadSynchronous());
	}
}

void UEquipSlotWidget::OnRootClicked()
{
	OnSlotClicked.Broadcast(PartType);
}
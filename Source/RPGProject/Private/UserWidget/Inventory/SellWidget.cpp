// Fill out your copyright notice in the Description page of Project Settings.

#include "SellWidget.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Components/TextBlock.h"
#include "Public/InveotorySystem/BaseItem.h"
#include "Public/TimerManager.h"
#include "Engine/World.h"
#include "Components/Button.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/UserWidget/Inventory/InventoryWidget.h"
#include "Components/WrapBox.h"
#include "Components/Image.h"
#include "Public/UserWidget/Inventory/ShopWidget.h"
#include "Components/UniformGridPanel.h"

#define LOCTEXT_NAMESPACE "Throw"

void USellWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MinusButton->OnClicked.AddDynamic(this, &USellWidget::OnMinusButtonClicked);
	MinusButton->OnPressed.AddDynamic(this, &USellWidget::OnMinusButtonPressed);
	MinusButton->OnReleased.AddDynamic(this, &USellWidget::OnMinusButtonReleased);

	PlusButton->OnClicked.AddDynamic(this, &USellWidget::OnPlusButtonClicked);
	PlusButton->OnPressed.AddDynamic(this, &USellWidget::OnPlusButtonPressed);
	PlusButton->OnReleased.AddDynamic(this, &USellWidget::OnPlusButtonReleased);

	ConfirmButton->OnClicked.AddDynamic(this, &USellWidget::OnConfirmButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &USellWidget::OnCancelButtonClicked);
}

void USellWidget::Update(int Index)
{
	CurrentIndex = Index;
	if (!InventoryRef->BIsSlotEmpty(CurrentIndex)) {
		ItemInfo = InventoryRef->GetItemAtIndex(CurrentIndex)->ItemInfo;
		MaxAmount = InventoryRef->GetAmountAtIndex(CurrentIndex);
		NameText->SetText(FText::Format(LOCTEXT("Throw", "{0} x{1}"),
			ItemInfo.Name, MaxAmount));
		ThrowCount = 1;
		CountText->SetText(FText::AsNumber(ThrowCount));
		IconImage->SetBrushFromTexture(ItemInfo.Icon);
	}
	UpdatePrice();
}

void USellWidget::UpdatePrice()
{
	PriceText->SetText(FText::AsNumber(ItemInfo.Price*ThrowCount));
}

void USellWidget::IncreaseCount()
{
	ThrowCount = FMath::Clamp(ThrowCount + 1, 1, MaxAmount);
	CountText->SetText(FText::AsNumber(ThrowCount));
	UpdatePrice();
}

void USellWidget::DecreaseCount()
{
	ThrowCount = FMath::Clamp(ThrowCount - 1, 1, MaxAmount);
	CountText->SetText(FText::AsNumber(ThrowCount));
	UpdatePrice();
}

void USellWidget::OnMinusButtonClicked()
{

	ClikCount++;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CountToZero, this, &USellWidget::SetClickCountToZero, 0.3f, false);

	if (ClikCount > 1) {
		ClikCount = 0;
		ThrowCount = 1;
		CountText->SetText(FText::AsNumber(ThrowCount));
		UpdatePrice();
	}
	else {
		DecreaseCount();
	}

}

void USellWidget::OnPlusButtonClicked()
{
	ClikCount++;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CountToZero, this, &USellWidget::SetClickCountToZero, 0.3f, false);

	if (ClikCount > 1) {
		ClikCount = 0;
		ThrowCount = MaxAmount;
		CountText->SetText(FText::AsNumber(ThrowCount));
		UpdatePrice();
	}
	else {
		IncreaseCount();
	}
}

void USellWidget::OnMinusButtonPressed()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Decrease, this, &USellWidget::DecreaseCount, 0.1f, true, 1.0f);
}

void USellWidget::OnPlusButtonPressed()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Increase, this, &USellWidget::IncreaseCount, 0.1f, true, 1.0f);
}

void USellWidget::OnMinusButtonReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Decrease);
}

void USellWidget::OnPlusButtonReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Increase);
}

void USellWidget::OnConfirmButtonClicked()
{
	if (InventoryRef->RemoveItemAtIndex(CurrentIndex, ThrowCount)) {
		InventoryRef->PlayerCharacter->IncreaseCoin(ThrowCount*ItemInfo.Price);
		this->SetVisibility(ESlateVisibility::Hidden);
		InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(true);
		ShopWidget->GridPanel->SetIsEnabled(true);
		ShopWidget->UpdateAllItems();
	}
}

void USellWidget::OnCancelButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(true);
	ShopWidget->GridPanel->SetIsEnabled(true);
}

void USellWidget::SetClickCountToZero()
{
	ClikCount = 0;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CountToZero);
}

#undef LOCTEXT_NAMESPACE


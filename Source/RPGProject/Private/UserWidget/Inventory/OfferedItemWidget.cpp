// Fill out your copyright notice in the Description page of Project Settings.

#include "OfferedItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/InveotorySystem/BaseItem.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Public/UserWidget/Inventory/ShopWidget.h"

void UOfferedItemWidget::UpdatePrice()
{
	PriceText->SetText(FText::AsNumber(ItemInfo.Price*CurrentAmount));
	FLinearColor PriceTextColor;
	if (Inventory->PlayerCharacter->CurrentCoin >= ItemInfo.Price*CurrentAmount) {
		BuyButton->SetIsEnabled(true);
		PriceTextColor = FLinearColor(1,0.48,0,1);
	}
	else {
		BuyButton->SetIsEnabled(false);
		PriceTextColor = FLinearColor::Red;
	}
	PriceText->SetColorAndOpacity(PriceTextColor);
}

void UOfferedItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AmountSlider->OnValueChanged.AddDynamic(this,&UOfferedItemWidget::OnAmountSliderValueChanged);
	BuyButton->OnClicked.AddDynamic(this,&UOfferedItemWidget::OnBuyButtonClicked);
}

void UOfferedItemWidget::OnAmountSliderValueChanged(float Value)
{
	CurrentAmount=FMath::Lerp(1,MaxAmount,Value);
	AmountText->SetText(FText::AsNumber(CurrentAmount));
	UpdatePrice();
}

void UOfferedItemWidget::OnBuyButtonClicked()
{
	int LeftAmount = Inventory->AddItem(AssignedItem, CurrentAmount);
	if ( LeftAmount< CurrentAmount) {
		Inventory->PlayerCharacter->DecreaseCoin((CurrentAmount - LeftAmount)*ItemInfo.Price);
		UpdatePrice();
		ShopWidget->UpdateAllItems();
	}
}

void UOfferedItemWidget::Init(TSubclassOf<ABaseItem> Item, class AInventory* InventoryRef, class UShopWidget* ShopWidgetRef)
{
	this->Inventory = InventoryRef;
	this->ShopWidget = ShopWidgetRef;
	AssignedItem = Item;
	ItemInfo=AssignedItem->GetDefaultObject<ABaseItem>()->ItemInfo;
	IconImage->SetBrushFromTexture(ItemInfo.Icon);
	NameText->SetText(ItemInfo.Name);

	if (ItemInfo.bCanBeStacked) {
		AmountText->SetText(FText::AsNumber(CurrentAmount));
		AmountText->SetVisibility(ESlateVisibility::Visible);
		AmountSlider->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		AmountText->SetVisibility(ESlateVisibility::Hidden);
		AmountSlider->SetVisibility(ESlateVisibility::Hidden);
		CurrentAmount = 1;
	}

	UpdatePrice();
}

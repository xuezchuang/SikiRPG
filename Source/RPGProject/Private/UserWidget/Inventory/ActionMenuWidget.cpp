// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/UserWidget/Inventory/ActionMenuWidget.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Public/InveotorySystem/BaseItem.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Public/UserWidget/Inventory/ThrowWidget.h"
#include "Components/WrapBox.h"
#include "Public/UserWidget/Inventory/InventoryWidget.h"

void UActionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UseItemButton->OnClicked.AddDynamic(this,&UActionMenuWidget::OnUseButtonClicked);
	ThrowAwayButton->OnClicked.AddDynamic(this,&UActionMenuWidget::OnThrowButtonClicked);
	SplitStacksButton->OnClicked.AddDynamic(this,&UActionMenuWidget::OnSplitButtonClicked);
	CancleButton->OnClicked.AddDynamic(this,&UActionMenuWidget::OnCancleButtonClicked);
}

void UActionMenuWidget::Update(int Index)
{
	CurrentIndex = Index;
	ItemInfo = InventoryRef->GetItemAtIndex(Index)->ItemInfo;
	Amount = InventoryRef->GetAmountAtIndex(Index);

	if (ItemInfo.bCanBeUsed) {
		UseText->SetText(ItemInfo.UseText);
		UseItemButton->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		UseItemButton->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (Amount > 1 && ItemInfo.bCanBeStacked) {
		SplitStacksButton->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		SplitStacksButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UActionMenuWidget::OnUseButtonClicked()
{
	InventoryRef->UseItemAtIndex(CurrentIndex);
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UActionMenuWidget::OnThrowButtonClicked()
{
	if (!ItemInfo.bCanBeStacked&&Amount > 0) {
		InventoryRef->RemoveItemAtIndex(CurrentIndex, 1);
	}
	else {
		InventoryRef->PlayerCharacter->MainUserWidget->ThrowWidget->Update(CurrentIndex);	
		InventoryRef->PlayerCharacter->MainUserWidget->ThrowWidget->SetVisibility(ESlateVisibility::Visible);
	}
	InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(false);
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UActionMenuWidget::OnSplitButtonClicked()
{
	InventoryRef->SplitStack(CurrentIndex,int(Amount/2));
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UActionMenuWidget::OnCancleButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}


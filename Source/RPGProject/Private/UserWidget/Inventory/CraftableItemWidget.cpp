// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftableItemWidget.h"
#include "Components/TextBlock.h"
#include "Public/InveotorySystem/BaseItem.h"
#include "Public/UserWidget/Inventory/CraftingMenuWidget.h"
#include "Components/Button.h"

void UCraftableItemWidget::OnSelectButtonClicked()
{
	CraftingMenu->OnCraftableCliked(this);
}

void UCraftableItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SelectButton->OnClicked.AddDynamic(this,&UCraftableItemWidget::OnSelectButtonClicked);
}

void UCraftableItemWidget::Init(TSubclassOf<class ABaseItem> ItemRef, class UCraftingMenuWidget* CraftingMenuRef)
{
	this->Item = ItemRef;
	this->CraftingMenu = CraftingMenuRef;
	NameText->SetText(ItemRef->GetDefaultObject<ABaseItem>()->ItemInfo.Name);
}

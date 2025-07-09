// Fill out your copyright notice in the Description page of Project Settings.

#include "ShopWidget.h"
#include "Components/TextBlock.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/InveotorySystem/Merchant.h"
#include "Public/UserWidget/Inventory/OfferedItemWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Button.h"
#include "Public/UserWidget/Inventory/ItemDragDropOperation.h"
#include "Public/UserWidget/Inventory/InventorySlotWidget.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Public/UserWidget/Inventory/SellWidget.h"
#include "Components/WrapBox.h"
#include "Public/UserWidget/Inventory/InventoryWidget.h"
#include "Public/UserWidget/MainUserWidget.h"

void UShopWidget::OnCloseButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	Merchant->bShopOpen = false;
}

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this, &UShopWidget::OnCloseButtonClicked);
}

bool UShopWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (Cast<UItemDragDropOperation>(InOperation))
	{
		UInventorySlotWidget* tSlot = Cast<UItemDragDropOperation>(InOperation)->DraggedSlot;
		if (tSlot->ItemInfo.Category == EItemCategories::QuestItem)
		{
			return false;
		}
		else
		{
			if (!tSlot->ItemInfo.bCanBeStacked && tSlot->Amount > 0)
			{
				if (PlayerCharacter->InventoryRef->RemoveItemAtIndex(tSlot->SlotIndex, 1))
				{
					PlayerCharacter->IncreaseCoin(tSlot->ItemInfo.Price);
					UpdateAllItems();
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				SellWidget->Update(tSlot->SlotIndex);
				SellWidget->SetVisibility(ESlateVisibility::Visible);
				PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(false);
				this->GridPanel->SetIsEnabled(false);
				return true;
			}
		}
	}
	else
	{
		return false;
	}
}

void UShopWidget::UpdateCoin()
{
	CoinText->SetText(FText::AsNumber(PlayerCharacter->CurrentCoin));
}

void UShopWidget::GenerateItemList()
{
	OfferedItemWidgets.Empty();
	GridPanel->ClearChildren();
	for (int i = 0; i < Merchant->OfferedItems.Num(); i++)
	{
		UOfferedItemWidget* OfferedItemWidget = CreateWidget<UOfferedItemWidget>(GetWorld(), LoadClass<UOfferedItemWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_OfferedItem.WBP_OfferedItem_C'")));
		OfferedItemWidget->Init(Merchant->OfferedItems[i], PlayerCharacter->InventoryRef, this);
		OfferedItemWidgets.Add(OfferedItemWidget);
		UPanelSlot* tSlot = GridPanel->AddChild(OfferedItemWidget);
		Cast<UUniformGridSlot>(tSlot)->SetRow(i / 2);
		Cast<UUniformGridSlot>(tSlot)->SetColumn(i % 2);
	}
}

void UShopWidget::UpdateAllItems()
{
	for (UOfferedItemWidget* ItemWidget : OfferedItemWidgets)
	{
		ItemWidget->UpdatePrice();
	}
}

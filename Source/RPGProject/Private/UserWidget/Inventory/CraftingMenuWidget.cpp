// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingMenuWidget.h"
#include "Components/ScrollBox.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Public/InveotorySystem/BaseItem.h"
#include "Public/UserWidget/Inventory/CraftableItemWidget.h"
#include "Public/UserWidget/Inventory/RecipeEntryWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UCraftingMenuWidget::OnCloseButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UCraftingMenuWidget::OnCraftButtonClicked()
{
	if (Inventory->AddItem(CurrentItem, 1) == 0)
	{
		for (FInventorySlot tSlot : CurrentItemInfo.Recipe)
		{
			Inventory->RemoveItem(tSlot.ItemClass, tSlot.Amount);
		}
	}
	CraftButton->SetIsEnabled(BCanBeCrafted());
}

void UCraftingMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this, &UCraftingMenuWidget::OnCloseButtonClicked);
	CraftButton->OnClicked.AddDynamic(this, &UCraftingMenuWidget::OnCraftButtonClicked);
}

void UCraftingMenuWidget::OnCraftableCliked(class UCraftableItemWidget* ClickedItem)
{
	if (CurrentCraftable)
	{
		CurrentCraftable->SelectButton->SetIsEnabled(true);
		CurrentCraftable = ClickedItem;
		CurrentCraftable->SelectButton->SetIsEnabled(false);
		UpdateDetailWindow(CurrentCraftable->Item);
	}
	else
	{
		CurrentCraftable = ClickedItem;
		CurrentCraftable->SelectButton->SetIsEnabled(false);
		UpdateDetailWindow(CurrentCraftable->Item);
	}
}

void UCraftingMenuWidget::GenerateCraftableList()
{
	CraftableItemsScrollBox->ClearChildren();
	for (TSubclassOf<ABaseItem> Item : Inventory->CraftableItems)
	{
		UCraftableItemWidget* CraftableItemWidget = CreateWidget<UCraftableItemWidget>(GetWorld(), LoadClass<UCraftableItemWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_CraftableItem.WBP_CraftableItem_C'")));
		CraftableItemWidget->Init(Item, this);
		CraftableItemsScrollBox->AddChild(CraftableItemWidget);
	}
}

void UCraftingMenuWidget::GenerateRecipeEntries()
{
	RecipeEntriesList->ClearChildren();
	RecipeEntries.Empty();

	for (FInventorySlot tSlot : CurrentItemInfo.Recipe)
	{
		URecipeEntryWidget* RecipeEntryWidget = CreateWidget<URecipeEntryWidget>(GetWorld(), LoadClass<URecipeEntryWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_RecipeEntry.WBP_RecipeEntry_C'")));
		RecipeEntryWidget->Item = tSlot.ItemClass;
		RecipeEntryWidget->RequiredAmount = tSlot.Amount;
		RecipeEntryWidget->CraftingMenu = this;
		RecipeEntryWidget->Update();
		RecipeEntries.Add(RecipeEntryWidget);
		RecipeEntriesList->AddChild(RecipeEntryWidget);
	}
}

bool UCraftingMenuWidget::BCanBeCrafted()
{
	bool TempBool = true;
	for (URecipeEntryWidget* Recipe : RecipeEntries)
	{
		if (Recipe->CurrentAmount < Recipe->RequiredAmount)
		{
			TempBool = false;
			break;
		}
	}
	return TempBool;
}

void UCraftingMenuWidget::UpdateDetailWindow(TSubclassOf<class ABaseItem> Item)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateDetailWindow"));
	if (Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item"));
		if (Item == CurrentItem)
		{
			for (URecipeEntryWidget* Recipe : RecipeEntries)
			{
				Recipe->Update();
			}
			CraftButton->SetIsEnabled(BCanBeCrafted());
			DetailBorder->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			CurrentItem = Item;
			CurrentItemInfo = Item->GetDefaultObject<ABaseItem>()->ItemInfo;
			IconImage->SetBrushFromTexture(CurrentItemInfo.Icon);
			NameText->SetText(CurrentItemInfo.Name);
			DescriptionText->SetText(CurrentItemInfo.Description);
			GenerateRecipeEntries();
			CraftButton->SetIsEnabled(BCanBeCrafted());
			DetailBorder->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		CurrentItem = nullptr;
		RecipeEntriesList->ClearChildren();
		DetailBorder->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCraftingMenuWidget::InitCraftingMenu(class AInventory* InventoryRef)
{
	this->Inventory = InventoryRef;
	GenerateCraftableList();
	UpdateDetailWindow(nullptr);
}

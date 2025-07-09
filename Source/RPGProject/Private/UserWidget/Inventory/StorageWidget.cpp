// Fill out your copyright notice in the Description page of Project Settings.

#include "StorageWidget.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "Public/InveotorySystem/Storage.h"
#include "Public/UserWidget/Inventory/StorageSlotWidget.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Components/Border.h"

void UStorageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this, &UStorageWidget::OnCloseButtonClicked);
}

void UStorageWidget::OnCloseButtonClicked()
{
	StorageRef->CloseStorage();
}

void UStorageWidget::GenerateSlotWidgets(AStorage* tStorageRef)
{
	if (StorageRef == nullptr)
	{
		this->StorageRef = tStorageRef;
	}
	else
	{
		if (tStorageRef == this->StorageRef)
		{
			return;
		}
		else
		{
			this->StorageRef = tStorageRef;
		}
	}

	StorageBox->ClearChildren();
	StorageSlotWidgets.Empty();
	for (int i = 0; i < tStorageRef->Slots.Num(); i++)
	{
		UStorageSlotWidget* StorageSlotWidget = CreateWidget<UStorageSlotWidget>(GetWorld(), LoadClass<UStorageSlotWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_StorageSlot.WBP_StorageSlot_C'")));
		StorageSlotWidget->StorageRef = tStorageRef;
		StorageSlotWidget->SlotIndex = i;
		StorageSlotWidget->StorageWidget = this;
		StorageSlotWidget->PlayerInventory = PlayerInventory;
		StorageSlotWidget->UpdateSlot();
		StorageBox->AddChild(StorageSlotWidget);
		StorageSlotWidgets.Add(StorageSlotWidget);
	}
}

void UStorageWidget::OnSlotClicked(int Index)
{
	int AmountToRemove = StorageRef->Slots[Index].Amount;
	int RestAmount = PlayerInventory->AddItem(StorageRef->Slots[Index].ItemClass, AmountToRemove);
	if (RestAmount < AmountToRemove)
	{
		StorageRef->RemoveItemAtIndex(Index, AmountToRemove - RestAmount);
	}
	StorageSlotWidgets[Index]->Base->SetBrushColor(FLinearColor(1, 1, 1, 0));
}

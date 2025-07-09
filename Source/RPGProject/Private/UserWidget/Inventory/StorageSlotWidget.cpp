// Fill out your copyright notice in the Description page of Project Settings.

#include "StorageSlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Public/InveotorySystem/BaseItem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Public/UserWidget/Inventory/ItemDragWidget.h"
#include "Public/UserWidget/Inventory/ItemDragDropOperation.h"
#include "Public/Character/RBaseCharacter.h"
#include "Components/Border.h"
#include "Public/UserWidget/Inventory/ItemDetailWidget.h"
#include "Public/UserWidget/Inventory/ActionMenuWidget.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Public/UserWidget/Inventory/InventoryWidget.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Public/InveotorySystem/Storage.h"
#include "Public/UserWidget/Inventory/StorageWidget.h"
#include "Public/UserWidget/Inventory/StorageSlotDragDropOperation.h"
#include "Public/UserWidget/Inventory/InventorySlotWidget.h"

void UStorageSlotWidget::UpdateSlot()
{
	if(StorageRef->BIsSlotEmpty(SlotIndex))
	{
		SlotButton->SetIsEnabled(false);
		IconImage->SetVisibility(ESlateVisibility::Hidden);
		AmountText->SetVisibility(ESlateVisibility::Hidden);
		SlotButton->SetToolTip(nullptr);

	}
	else
	{
		SlotButton->SetIsEnabled(true);
		ItemInfo = StorageRef->GetItemAtIndex(SlotIndex)->ItemInfo;
		Amount = StorageRef->GetAmountAtIndex(SlotIndex);
		IconImage->SetBrushFromTexture(ItemInfo.Icon);
		IconImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		if(ItemInfo.bCanBeStacked)
		{
			AmountText->SetText(FText::AsNumber(Amount));
			AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			AmountText->SetVisibility(ESlateVisibility::Hidden);
		}
		if(!ItemDetail)
		{

			ItemDetail = CreateWidget<UItemDetailWidget>(GetWorld(), LoadClass<UItemDetailWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_ItemDetail.WBP_ItemDetail_C'")));
		}
		ItemDetail->ItemInfo = ItemInfo;
		ItemDetail->Amount = Amount;
		SlotButton->SetToolTip(ItemDetail);
		ItemDetail->Update();
	}
}

void UStorageSlotWidget::OnSlotButtonClicked()
{
	StorageWidget->OnSlotClicked(SlotIndex);
}


void UStorageSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SlotButton->OnClicked.AddDynamic(this, &UStorageSlotWidget::OnSlotButtonClicked);
}

FReply UStorageSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{

		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton).NativeReply;
	}
	else
	{
		return FReply::Handled();
	}
}

void UStorageSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if(!StorageRef->BIsSlotEmpty(SlotIndex))
	{
		UItemDragWidget* ItemDrag = CreateWidget<UItemDragWidget>(GetWorld(), LoadClass<UItemDragWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_ItemDrag.WBP_ItemDrag_C'")));
		ItemDrag->Init(Amount, StorageRef->GetItemAtIndex(SlotIndex)->ItemInfo.Icon);

		UDragDropOperation* StorageSlotOp = UWidgetBlueprintLibrary::CreateDragDropOperation(StorageSlotDrag);
		StorageSlotOp->DefaultDragVisual = ItemDrag;
		OutOperation = StorageSlotOp;

		if(Cast<UStorageSlotDragDropOperation>(OutOperation))
		{
			Cast<UStorageSlotDragDropOperation>(OutOperation)->Slot = this;
		}
	}
}

bool UStorageSlotWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	if(bDraggedOver)
	{
		return true;
	}
	else
	{
		if(Cast<UItemDragDropOperation>(InOperation))
		{
			bDraggedOver = true;
			Base->SetBrushColor(FLinearColor(1.0f, 0.82f, 0.0f, 0.5f));
			return true;
		}
		else if(Cast<UStorageSlotDragDropOperation>(InOperation))
		{
			if(Cast<UStorageSlotDragDropOperation>(InOperation)->Slot != this)
			{
				bDraggedOver = true;
				Base->SetBrushColor(FLinearColor(1.0f, 0.82f, 0.0f, 0.5f));
				return true;
			}
		}
		return false;
	}
}

void UStorageSlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	if(bDraggedOver)
	{

		if(Cast<UItemDragDropOperation>(InOperation) || Cast<UStorageSlotDragDropOperation>(InOperation))
		{
			bDraggedOver = false;
			Base->SetBrushColor(FLinearColor(1, 1, 1, 0));
		}

	}
}

bool UStorageSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if(Cast<UStorageSlotDragDropOperation>(InOperation))
	{
		if(StorageRef->AddToIndex(Cast<UStorageSlotDragDropOperation>(InOperation)->Slot->SlotIndex, SlotIndex))
		{
			return true;
		}
		else
		{
			return StorageRef->SwapSlot(Cast<UStorageSlotDragDropOperation>(InOperation)->Slot->SlotIndex, SlotIndex);
		}
		bDraggedOver = false;
		Base->SetBrushColor(FLinearColor(1, 1, 1, 0));
	}
	else if(Cast<UItemDragDropOperation>(InOperation))
	{
		return StorageRef->MoveFromInvToStorageByIndex(Cast<UItemDragDropOperation>(InOperation)->DraggedSlot->InvetoryRef, Cast<UItemDragDropOperation>(InOperation)->DraggedSlot->SlotIndex, SlotIndex);
		bDraggedOver = false;
		Base->SetBrushColor(FLinearColor(1, 1, 1, 0));
	}
	else
	{
		return false;
		bDraggedOver = false;
		Base->SetBrushColor(FLinearColor(1, 1, 1, 0));
	}
}

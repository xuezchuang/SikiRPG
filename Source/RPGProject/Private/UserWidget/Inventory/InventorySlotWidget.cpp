
// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/UserWidget/Inventory/InventorySlotWidget.h"
#include "Public/InveotorySystem/Inventory.h"
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
#include "Public/UserWidget/Inventory/StorageSlotDragDropOperation.h"
#include "Public/UserWidget/Inventory/StorageSlotWidget.h"

void UInventorySlotWidget::UpdateSlot()
{
	if (InvetoryRef->BIsSlotEmpty(SlotIndex))
	{
		SlotButton->SetIsEnabled(false);
		IconImage->SetVisibility(ESlateVisibility::Hidden);
		AmountText->SetVisibility(ESlateVisibility::Hidden);
		SlotButton->SetToolTip(nullptr);

	}
	else
	{
		SlotButton->SetIsEnabled(true);
		ItemInfo = InvetoryRef->GetItemAtIndex(SlotIndex)->ItemInfo;
		Amount = InvetoryRef->GetAmountAtIndex(SlotIndex);
		IconImage->SetBrushFromTexture(ItemInfo.Icon);
		IconImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (ItemInfo.bCanBeStacked)
		{
			AmountText->SetText(FText::AsNumber(Amount));
			AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			AmountText->SetVisibility(ESlateVisibility::Hidden);
		}
		if (!ItemDetail)
		{

			ItemDetail = CreateWidget<UItemDetailWidget>(GetWorld(), LoadClass<UItemDetailWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_ItemDetail.WBP_ItemDetail_C'")));
		}
		ItemDetail->ItemInfo = ItemInfo;
		ItemDetail->Amount = Amount;
		SlotButton->SetToolTip(ItemDetail);
		ItemDetail->Update();
	}
}

void UInventorySlotWidget::OnSlotButtonClicked()
{
	ClickCount++;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CountToZero, this, &UInventorySlotWidget::SetCountToZero, 0.3f, false);

	if (ClickCount > 1)
	{
		InvetoryRef->UseItemAtIndex(SlotIndex);
		ClickCount = 0;
		InvetoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CountToZero);
	}


}

void UInventorySlotWidget::SetCountToZero()
{
	ClickCount = 0;
	InvetoryRef->UpdateActionMenuPosition(this);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CountToZero);
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnSlotButtonClicked);
}

//FReply UInventorySlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
//	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && ItemInfo.bCanBeUsed) {
//		InvetoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->SetVisibility(ESlateVisibility::Hidden);
//		InvetoryRef->UseItemAtIndex(SlotIndex);
//
//		return FReply::Handled();
//	}
//	else {
//		return FReply::Handled();
//	}
//
//}

//FReply UInventorySlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
//	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
//		if (!InvetoryRef->BIsSlotEmpty(SlotIndex))
//			InvetoryRef->UpdateActionMenuPosition(this);
//		return FReply::Handled();
//	}
//	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)) {
//	
//		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton).NativeReply;
//	}
//	else {
//		return FReply::Handled();
//	}
//
//}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{

		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton).NativeReply;
	}
	else
	{
		return FReply::Handled();
	}

}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (!InvetoryRef->BIsSlotEmpty(SlotIndex))
	{
		UItemDragWidget* ItemDrag = CreateWidget<UItemDragWidget>(GetWorld(), LoadClass<UItemDragWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_ItemDrag.WBP_ItemDrag_C'")));
		ItemDrag->Init(Amount, InvetoryRef->GetItemAtIndex(SlotIndex)->ItemInfo.Icon);

		UDragDropOperation* TempOp = UWidgetBlueprintLibrary::CreateDragDropOperation(ItemDragDropOp);
		TempOp->DefaultDragVisual = ItemDrag;
		OutOperation = TempOp;

		if (Cast<UItemDragDropOperation>(OutOperation))
		{
			Cast<UItemDragDropOperation>(OutOperation)->bShiftDown = InvetoryRef->PlayerCharacter->bShiftDown;
			Cast<UItemDragDropOperation>(OutOperation)->DraggedSlot = this;
		}
	}
}

bool UInventorySlotWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	if (bDraggedOver)
	{
		return true;
	}
	else
	{
		if (Cast<UItemDragDropOperation>(InOperation))
		{
			if (Cast<UItemDragDropOperation>(InOperation)->DraggedSlot != this)
			{
				bDraggedOver = true;
				Base->SetBrushColor(FLinearColor(1.0f, 0.82f, 0.0f, 0.5f));
			}
			return true;
		}
		else
		{
			if (Cast<UStorageSlotDragDropOperation>(InOperation))
			{
				bDraggedOver = true;
				Base->SetBrushColor(FLinearColor(1.0f, 0.82f, 0.0f, 0.5f));
			}
			return true;
		}
	}
}

void UInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	if (bDraggedOver)
	{
		if (Cast<UItemDragDropOperation>(InOperation) || Cast<UStorageSlotDragDropOperation>(InOperation))
		{
			Base->SetBrushColor(FLinearColor(1, 1, 1, 0));
			bDraggedOver = false;
		}
	}
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (Cast<UItemDragDropOperation>(InOperation))
	{
		UInventorySlotWidget* tSlot = Cast<UItemDragDropOperation>(InOperation)->DraggedSlot;
		if (tSlot != this)
		{
			bDraggedOver = false;
			Base->SetBrushColor(FLinearColor(1, 1, 1, 0));
			if (InvetoryRef->AddToIndex(tSlot->SlotIndex, SlotIndex))
			{
				return true;
			}
			else
			{
				if (Cast<UItemDragDropOperation>(InOperation)->bShiftDown)
				{
					InvetoryRef->SplitStackToIndex(tSlot->SlotIndex, SlotIndex, tSlot->Amount / 2);
				}
				else
				{
					InvetoryRef->SwapSlot(tSlot->SlotIndex, SlotIndex);
				}
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	else if (Cast<UStorageSlotDragDropOperation>(InOperation))
	{
		return InvetoryRef->MoveFromStorageToInvByIndex(Cast<UStorageSlotDragDropOperation>(InOperation)->Slot->StorageRef, Cast<UStorageSlotDragDropOperation>(InOperation)->Slot->SlotIndex, SlotIndex);
	}
	else
	{
		return false;
	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/UserWidget/Inventory/InventoryWidget.h"
#include "Components/WrapBox.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Public/UserWidget/Inventory/InventorySlotWidget.h"
#include "Components/Button.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Public/UserWidget/Inventory/InventoryDragDropOperation.h"
#include "Public/Other/StaticLibrary.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this,&UInventoryWidget::OnCloseButtonClicked);
	SortButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnSortButtonClicked);
	for (ESortType Type : UStaticLibrary::EnumGetList<ESortType>())
	{
		SortCategoryBox->AddOption(UStaticLibrary::GetEnumValueAsString<ESortType>("ESortType", Type));
	}
	SortCategoryBox->SetSelectedOption(SortCategoryBox->GetOptionAtIndex(0));
}

void UInventoryWidget::OnCloseButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	InventoryRef->PlayerCharacter->MainUserWidget->bInventoryVisibling = false;
}

FReply UInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)) {
		DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton).NativeReply;
	}
	else {
		return FReply::Handled();
	}
}

void UInventoryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry,InMouseEvent,OutOperation);

	UDragDropOperation* TempOp= UWidgetBlueprintLibrary::CreateDragDropOperation(InventoryDragDropOperation);
	TempOp->DefaultDragVisual = this;
	TempOp->Pivot = EDragPivot::MouseDown;
	OutOperation = TempOp;

	if (Cast<UInventoryDragDropOperation>(OutOperation)) {	
		Cast<UInventoryDragDropOperation>(OutOperation)->WidgetToDrag = this;
		Cast<UInventoryDragDropOperation>(OutOperation)->MouseOffset = DragOffset;
	}

	this->RemoveFromParent();
}

void UInventoryWidget::OnSortButtonClicked()
{
	/*if (BReversed)
	{
		InventoryRef->SortInventory(static_cast<ESortType>(SortCategoryBox->FindOptionIndex(SortCategoryBox->GetSelectedOption())), true);
		BReversed = false;
	}
	else {
		InventoryRef->SortInventory(static_cast<ESortType>(SortCategoryBox->FindOptionIndex(SortCategoryBox->GetSelectedOption())), false);
		BReversed = true;
	}*/
	InventoryRef->SortInventory(static_cast<ESortType>(SortCategoryBox->FindOptionIndex(SortCategoryBox->GetSelectedOption())), BReversed);
	BReversed = !BReversed;
}

void UInventoryWidget::GenerateSlotWidgets()
{
	InventoryBox->ClearChildren();
	InventorySlotWidgets.Empty();
	for (int i = 0; i < InventoryRef->Slots.Num(); i++)
	{
		UInventorySlotWidget* InventorySlotWidget = CreateWidget<UInventorySlotWidget>(GetWorld(), LoadClass<UInventorySlotWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_InventorySlot.WBP_InventorySlot_C'")));
		InventorySlotWidget->InvetoryRef = InventoryRef;
		InventorySlotWidget->SlotIndex = i;
		InventorySlotWidget->UpdateSlot();
		InventoryBox->AddChild(InventorySlotWidget);
		InventorySlotWidgets.Add(InventorySlotWidget);
	}
}

void UInventoryWidget::UpdateWeight(float CurrentWeight, float TotalWeight)
{
	CurrentWeightText->SetText(FText::AsNumber(CurrentWeight));
	TotalWeightText->SetText(FText::AsNumber(TotalWeight));

	if (CurrentWeight > TotalWeight) {
		CurrentWeightText->SetColorAndOpacity(FLinearColor::Red);
		TotalWeightText->SetColorAndOpacity(FLinearColor::Red);
	}
	else {
		CurrentWeightText->SetColorAndOpacity(FLinearColor::White);
		TotalWeightText->SetColorAndOpacity(FLinearColor::White);
	}
}

void UInventoryWidget::UpdateCoinText(int Coin)
{
	CoinText->SetText(FText::AsNumber(Coin));
}

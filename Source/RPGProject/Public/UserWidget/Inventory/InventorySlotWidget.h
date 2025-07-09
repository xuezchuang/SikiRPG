// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta=(BindWidget))
	class UButton* SlotButton;
	UPROPERTY(Meta = (BindWidget))
	class UImage* IconImage;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* AmountText;
	UPROPERTY(Meta = (BindWidget))
	class UBorder* Base;

protected:
	UPROPERTY(EditAnywhere,Category=Drag)
	TSubclassOf<class UDragDropOperation> ItemDragDropOp;
public:
	int SlotIndex;

	int Amount;

	FItemInfo ItemInfo;
	class AInventory* InvetoryRef;
	void UpdateSlot();

	bool bDraggedOver;

	class UItemDetailWidget* ItemDetail;

	UFUNCTION()
		void OnSlotButtonClicked();

	int ClickCount;

	FTimerHandle TimerHandle_CountToZero;

	void SetCountToZero();

	virtual void NativeConstruct() override;
	//virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "StorageSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UStorageSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	

		UPROPERTY(Meta = (BindWidget))
		class UButton* SlotButton;
	UPROPERTY(Meta = (BindWidget))
		class UImage* IconImage;
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* AmountText;


protected:
	UPROPERTY(EditAnywhere, Category = DragDrop)
		TSubclassOf<class UDragDropOperation> StorageSlotDrag;
public:
	UPROPERTY(Meta = (BindWidget))
		class UBorder* Base;
	int SlotIndex;

	int Amount;

	FItemInfo ItemInfo;
	class AStorage* StorageRef;
	class UStorageWidget* StorageWidget;
	class AInventory* PlayerInventory;
	void UpdateSlot();

	bool bDraggedOver;

	class UItemDetailWidget* ItemDetail;

	UFUNCTION()
		void OnSlotButtonClicked();


	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	
};

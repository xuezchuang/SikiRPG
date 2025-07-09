// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* CurrentWeightText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TotalWeightText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* CoinText;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseButtonClicked();

protected:
	UPROPERTY(EditAnywhere,Category=DragDrop)
	TSubclassOf<class UDragDropOperation> InventoryDragDropOperation;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;


	bool BReversed = false;
	UFUNCTION()
		void OnSortButtonClicked();
	
public:

	UPROPERTY(Meta = (BindWidget))
		class UActionMenuWidget* ActionMenu;


	UPROPERTY(Meta = (BindWidget))
		class UComboBoxString* SortCategoryBox;
	UPROPERTY(Meta = (BindWidget))
		class UButton* SortButton;

	TArray<class UInventorySlotWidget*> InventorySlotWidgets;

	class AInventory* InventoryRef;

	void GenerateSlotWidgets();

	FVector2D DragOffset;

	UPROPERTY(Meta = (BindWidget))
		class UWrapBox* InventoryBox;

	void UpdateWeight(float CurrentWeight,float TotalWeight);

	void UpdateCoinText(int Coin);
};

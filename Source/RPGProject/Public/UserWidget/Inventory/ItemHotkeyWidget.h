// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemHotkeyWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UItemHotkeyWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta=(BindWidget))
	class UImage* IconImage;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* AmountText;
	UPROPERTY(Meta = (BindWidget))
	class UBorder* Bg;

public:
	bool bEmpty = true;//快捷键上是否有物品
	int InventoryIndex;//拖过来的物品在背包的哪个插槽上，是插槽的索引
	class AInventory* Inventory;
	FLinearColor DefaultTint = FLinearColor(0.086275, 0.086275, 0.090196,1);
	FLinearColor DragOverTint = FLinearColor(1.0f,0.478f,0,1);
	bool bDraggedOver;

	void Update();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};

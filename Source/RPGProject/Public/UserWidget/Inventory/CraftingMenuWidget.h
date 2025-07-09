// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "CraftingMenuWidget.generated.h"

/**
 *
 */
UCLASS()
class RPGPROJECT_API UCraftingMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta = (BindWidget))
	class UButton* CloseButton;
	UPROPERTY(Meta = (BindWidget))
	class UScrollBox* CraftableItemsScrollBox;
	UPROPERTY(Meta = (BindWidget))
	class UImage* IconImage;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* NameText;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* DescriptionText;
	UPROPERTY(Meta = (BindWidget))
	class UButton* CraftButton;
	UPROPERTY(Meta = (BindWidget))
	class UVerticalBox* RecipeEntriesList;
	UPROPERTY(Meta = (BindWidget))
	class UBorder* DetailBorder;

	UFUNCTION()
	void OnCloseButtonClicked();

	UFUNCTION()
	void OnCraftButtonClicked();

	virtual void NativeConstruct() override;

public:
	class AInventory* Inventory;
	void OnCraftableCliked(class UCraftableItemWidget* ClickedItem);

	void GenerateCraftableList();

	class UCraftableItemWidget* CurrentCraftable;
	TSubclassOf<ABaseItem> CurrentItem;
	FItemInfo CurrentItemInfo;
	TArray<class URecipeEntryWidget*> RecipeEntries;
	void GenerateRecipeEntries();

	bool BCanBeCrafted();

	void UpdateDetailWindow(TSubclassOf<class ABaseItem> Item);

	void InitCraftingMenu(class AInventory* Inventory);
};

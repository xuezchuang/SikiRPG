// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "OfferedItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UOfferedItemWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta=(BindWidget))
	class UTextBlock* NameText;
	UPROPERTY(Meta = (BindWidget))
	class UImage* IconImage;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* PriceText;
	UPROPERTY(Meta = (BindWidget))
	class UButton* BuyButton;
	UPROPERTY(Meta = (BindWidget))
	class USlider* AmountSlider;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* AmountText;
	
	class AInventory* Inventory;

	TSubclassOf<class ABaseItem> AssignedItem;

	FItemInfo ItemInfo;

	int CurrentAmount = 1;
	UPROPERTY(EditAnywhere,Category=ItemInfo)
	int MaxAmount = 99;


	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnAmountSliderValueChanged(float Value);
	UFUNCTION()
	void OnBuyButtonClicked();

	class UShopWidget* ShopWidget;

public:
	void Init(TSubclassOf<ABaseItem> Item,class AInventory* InventoryRef,class UShopWidget* ShopWidgetRef);

	void UpdatePrice();
};

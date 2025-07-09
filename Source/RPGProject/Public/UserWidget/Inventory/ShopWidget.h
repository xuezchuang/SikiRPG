// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(Meta=(BindWidget))
		class UButton* CloseButton;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* CoinText;

	TArray<class UOfferedItemWidget*> OfferedItemWidgets;

	UFUNCTION()
		void OnCloseButtonClicked();

	virtual void NativeConstruct() override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
public:
	UPROPERTY(Meta = (BindWidget))
		class UUniformGridPanel* GridPanel;
	class ARBaseCharacter* PlayerCharacter;
	class AMerchant* Merchant;
	UPROPERTY(Meta = (BindWidget))
		class USellWidget* SellWidget;
	void UpdateCoin();

	void GenerateItemList();

	void UpdateAllItems();
};

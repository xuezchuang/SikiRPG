// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "ActionMenuWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UActionMenuWidget : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(Meta=(BindWidget))
		UButton* UseItemButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* ThrowAwayButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* SplitStacksButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* CancleButton;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* UseText;
	int CurrentIndex;
	FItemInfo ItemInfo;
	int Amount;


	UFUNCTION()
	void OnUseButtonClicked();
	UFUNCTION()
	void OnThrowButtonClicked();
	UFUNCTION()
	void OnSplitButtonClicked();
	UFUNCTION()
	void OnCancleButtonClicked();

	virtual void NativeConstruct() override;
public:

	void Update(int Index);
	class AInventory* InventoryRef;
};

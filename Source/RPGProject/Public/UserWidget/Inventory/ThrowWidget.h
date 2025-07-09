// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "ThrowWidget.generated.h"

class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UThrowWidget : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(Meta=(BindWidget))
		class UImage* IconImage;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* NameText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* CountText;

	UPROPERTY(Meta = (BindWidget))
	UButton* MinusButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* PlusButton;

	UPROPERTY(Meta = (BindWidget))
	UButton* ConfirmButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* CancelButton;

	int ThrowCount;//当前要丢弃的数量
	int MaxAmount;//插槽上的数量
	int CurrentIndex;

	FItemInfo ItemInfo;

	int ClikCount = 0;

	virtual void NativeConstruct() override;

	void IncreaseCount();

	void DecreaseCount();

	UFUNCTION()
	void OnMinusButtonClicked();
	UFUNCTION()
	void OnPlusButtonClicked();
	UFUNCTION()
	void OnMinusButtonPressed();
	UFUNCTION()
	void OnPlusButtonPressed();
	UFUNCTION()
	void OnMinusButtonReleased();
	UFUNCTION()
	void OnPlusButtonReleased();
	UFUNCTION()
	void OnConfirmButtonClicked();
	UFUNCTION()
	void OnCancelButtonClicked();

	FTimerHandle TimerHandle_Increase;
	FTimerHandle TimerHandle_Decrease;

	FTimerHandle TimerHandle_CountToZero;
	void SetClickCountToZero();

public:
	class AInventory* InventoryRef;

	void Update(int Index);
};

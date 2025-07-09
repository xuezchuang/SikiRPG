// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StorageWidget.generated.h"

class UTextBlock;

/**
 *
 */
UCLASS()
class RPGPROJECT_API UStorageWidget : public UUserWidget
{
	GENERATED_BODY()


	UPROPERTY(Meta = (BindWidget))
	class UButton* CloseButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseButtonClicked();

protected:


public:

	class AInventory* PlayerInventory;

	TArray<class UStorageSlotWidget*> StorageSlotWidgets;

	class AStorage* StorageRef;

	void GenerateSlotWidgets(class AStorage* tStorageRef);

	UPROPERTY(Meta = (BindWidget))
	class UWrapBox* StorageBox;

	void OnSlotClicked(int Index);
};

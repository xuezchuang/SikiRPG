// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemHotkey.generated.h"

/**
 *
 */
UCLASS()
class RPGPROJECT_API UItemHotkey : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(Meta = (BindWidget))
		class UTextBlock* KeyText;
	class AInventory* Inventory;

public:

	FKey Key;
	UPROPERTY(Meta = (BindWidget))
		class UItemHotkeyWidget* ItemHotkeySlot;
	void Init(FKey KeyRef,class AInventory* InventoryRef);;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "ItemDragWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UItemDragWidget : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(Meta=(BindWidget))
	class UTextBlock* AmountText;
	UPROPERTY(Meta = (BindWidget))
	class UImage* IconImage;
	
public:
	void Init(int Amount,UTexture2D* Icon);
};

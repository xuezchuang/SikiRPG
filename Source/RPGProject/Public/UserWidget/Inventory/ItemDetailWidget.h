// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "ItemDetailWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UItemDetailWidget : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(Meta=(BindWidget))
		UTextBlock* NameText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* AmountText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* DescriptionText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* CategoryText;
	UPROPERTY(Meta = (BindWidget))
	class UImage* IconImage;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TotalWeightText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* SingleWeightText;
public:
	FItemInfo ItemInfo;
	int Amount;

	void Update();
};

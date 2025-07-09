// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Public/InveotorySystem/InventoryEnum.h"
#include "InventoryStruct.generated.h"

USTRUCT()
struct FInventorySlot {
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = ItemInfo)
		TSubclassOf<class ABaseItem> ItemClass;
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	int Amount;
};


USTRUCT()
struct FItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category=ItemInfo)
	FText Name;
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	FText Description;
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	class UTexture2D* Icon;
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	bool bCanBeUsed = true;
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	FText UseText;//使用说明，比如药水：Drink，地图：Read
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	bool bCanBeStacked;//该物品是否能够被堆积
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	EItemCategories Category = EItemCategories::Consumle;
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	FString CompareName;
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	float Weight;
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	TArray<FInventorySlot> Recipe;//合成该物体所需要的信息（物品及数量）
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	float Price;
};

USTRUCT()
struct FSavedPickup
{
	GENERATED_BODY()

		UPROPERTY()
		int ID;//场景中所有需要保存的物体的ID，同一个类不同物品ID不同，如果该物体不需要保存，ID=-1
	UPROPERTY()
		int RestAmount;
};


/**
 * 
 */
UCLASS()
class RPGPROJECT_API UInventoryStruct : public UObject
{
	GENERATED_BODY()
	
	
	
	
};

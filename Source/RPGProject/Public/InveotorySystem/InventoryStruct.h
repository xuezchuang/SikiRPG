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
	FText UseText;//ʹ��˵��������ҩˮ��Drink����ͼ��Read
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	bool bCanBeStacked;//����Ʒ�Ƿ��ܹ����ѻ�
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	EItemCategories Category = EItemCategories::Consumle;
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	FString CompareName;
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	float Weight;
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	TArray<FInventorySlot> Recipe;//�ϳɸ���������Ҫ����Ϣ����Ʒ��������
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	float Price;
};

USTRUCT()
struct FSavedPickup
{
	GENERATED_BODY()

		UPROPERTY()
		int ID;//������������Ҫ����������ID��ͬһ���಻ͬ��ƷID��ͬ����������岻��Ҫ���棬ID=-1
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

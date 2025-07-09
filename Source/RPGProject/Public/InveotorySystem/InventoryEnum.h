// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryEnum.generated.h"

UENUM()
enum class EItemCategories {
	Consumle,
	Equipment,
	QuestItem,
	Readable,
	Material
};

UENUM()
enum class ESortType {
	Category,
	Amount,
	Name
};

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UInventoryEnum : public UObject
{
	GENERATED_BODY()
	
	
	
	
};

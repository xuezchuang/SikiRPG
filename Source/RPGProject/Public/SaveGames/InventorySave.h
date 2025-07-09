// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "InventorySave.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UInventorySave : public USaveGame
{
	GENERATED_BODY()
public:
		UPROPERTY()
		float SavedWeight;
	UPROPERTY()
	TArray<FInventorySlot> SavedSlots;
	UPROPERTY()
	TArray<FSavedPickup> SavedPickups;
	
};

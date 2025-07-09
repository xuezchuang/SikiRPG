// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "StorageSave.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UStorageSave : public USaveGame
{
	GENERATED_BODY()
public:
		UPROPERTY()
		TArray<FInventorySlot> SavedSlots;
	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "StorageSlotDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UStorageSlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	class UStorageSlotWidget* Slot;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InveotorySystem/BaseItem.h"
#include "ItemStaff.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AItemStaff : public ABaseItem
{
	GENERATED_BODY()

		virtual void OnUsed() override;
	
	
};

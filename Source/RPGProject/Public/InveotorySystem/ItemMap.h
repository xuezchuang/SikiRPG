// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InveotorySystem/BaseItem.h"
#include "ItemMap.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AItemMap : public ABaseItem
{
	GENERATED_BODY()

		virtual void OnUsed() override;
	
	class UReadableItemWidget* Map;
public:
	void OnReadOver();
};

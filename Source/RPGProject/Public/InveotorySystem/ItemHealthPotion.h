// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InveotorySystem/BaseItem.h"
#include "ItemHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AItemHealthPotion : public ABaseItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Category=Data)
	float HpDuration;
		virtual void OnUsed() override;
	
};

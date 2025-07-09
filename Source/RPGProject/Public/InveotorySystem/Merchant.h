// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/BaseNPC.h"
#include "Merchant.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AMerchant : public ABaseNPC
{
	GENERATED_BODY()

		class ARBaseCharacter* PlayerCharacter;

	 class UShopWidget* ShopWidget;

		virtual void BeginPlay() override;

		virtual void OnIteractWith(class ARBaseCharacter* Character) override;

		virtual void OnLeavePlayerRadius(class ARBaseCharacter* Character) override;
public:
	bool bShopOpen;
	UPROPERTY(EditAnywhere,Category=Item)
	TArray<TSubclassOf<class ABaseItem>> OfferedItems;
};

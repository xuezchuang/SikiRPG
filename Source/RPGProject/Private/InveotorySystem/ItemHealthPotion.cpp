// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/InveotorySystem/ItemHealthPotion.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Public/Character/RBaseCharacter.h"

void AItemHealthPotion::OnUsed()
{
	if (InventoryRef->PlayerCharacter->GetCurrentHp() < InventoryRef->PlayerCharacter->GetTotalHp()) {
		InventoryRef->PlayerCharacter->ChangeCurrentHp(HpDuration);
		InventoryRef->RemoveItemAtIndex(Index, 1);
		Destroy();
	}
	else {
		Destroy();
	}
}

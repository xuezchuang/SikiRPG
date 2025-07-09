// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemStaff.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Public/Character/RBaseCharacter.h"

void AItemStaff::OnUsed()
{
	if (!InventoryRef->PlayerCharacter->EquipItem(this)) {
		Destroy();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemHotkey.h"
#include "Components/TextBlock.h"

void UItemHotkey::Init(FKey KeyRef, class AInventory* InventoryRef)
{
	this->Inventory = InventoryRef;
	this->Key = KeyRef;
	KeyText->SetText(KeyRef.GetDisplayName());
}

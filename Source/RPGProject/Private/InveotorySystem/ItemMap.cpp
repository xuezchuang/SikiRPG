// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemMap.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Public/UserWidget/Inventory/InventoryWidget.h"
#include "Public/UserWidget/Inventory/ReadableItemWidget.h"
#include "Components/Border.h"

void AItemMap::OnUsed()
{
	InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->SetIsEnabled(false);
	Map=CreateWidget<UReadableItemWidget>(GetWorld(), LoadClass<UReadableItemWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_ItemReadable.WBP_ItemReadable_C'")));
	Map->ItemMap = this;
	InventoryRef->PlayerCharacter->MainUserWidget->ReadableContainer->AddChild(Map);
	FInputModeUIOnly InputMode;
	InventoryRef->PlayerCharacter->PC->SetInputMode(InputMode);
}

void AItemMap::OnReadOver()
{
	InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->SetIsEnabled(true);
	if(Map)
	Map->RemoveFromParent();
	FInputModeGameAndUI InputMode;
	InventoryRef->PlayerCharacter->PC->SetInputMode(InputMode);
}

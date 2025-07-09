// Fill out your copyright notice in the Description page of Project Settings.

#include "StoreKeeper.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Public/UserWidget/Inventory/StorageWidget.h"
#include "Public/InveotorySystem/Storage.h"

void AStoreKeeper::OnIteractWith(class ARBaseCharacter* Character)
{
	if (StorageRef==nullptr) {
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if(StorageClass)
		StorageRef=GetWorld()->SpawnActor<AStorage>(StorageClass, Params);
	}

	Character->MainUserWidget->StorageWidget->GenerateSlotWidgets(StorageRef);
	StorageRef->OpenStorage();
}

void AStoreKeeper::OnLeavePlayerRadius(class ARBaseCharacter* Character)
{
	if (StorageRef) {
		StorageRef->CloseStorage();
	}
}

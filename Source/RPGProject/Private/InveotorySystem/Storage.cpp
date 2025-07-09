// Fill out your copyright notice in the Description page of Project Settings.

#include "Storage.h"
#include "Public/InveotorySystem/BaseItem.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Public/UserWidget/Inventory/StorageSlotWidget.h"
#include "Public/UserWidget/Inventory/StorageWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Kismet/GameplayStatics.h"
#include "Public/SaveGames/StorageSave.h"
// Sets default values
AStorage::AStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStorage::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter =Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	StorageWidget = PlayerCharacter->MainUserWidget->StorageWidget;
	if (UGameplayStatics::DoesSaveGameExist(SavedSlotName, 0)) {
		LoadStorage();
	}
	Slots.SetNum(AmountOfSlots);
}

int AStorage::SearchEmptySlot()
{
	for (int i = 0; i < Slots.Num(); i++) {
		if (!Slots[i].ItemClass) {
			return i;
		}
	}
	return Slots.Num();
}

int AStorage::SearchFreeStack(TSubclassOf<class ABaseItem> Item)
{
	for (int i = 0; i < Slots.Num(); i++) {
		if (Slots[i].ItemClass&&Slots[i].ItemClass == Item && Slots[i].Amount < MaxStackSize) {
			return i;
		}
	}
	return Slots.Num();
}

// Called every frame
void AStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AStorage::AddItem(TSubclassOf<class ABaseItem> Item, int Amount)
{
	int FoundIndex = 0;
	//先根据物品本身属性来判断
	if (Item->GetDefaultObject<ABaseItem>()->ItemInfo.bCanBeStacked) {
		//该物品可以被堆积，此时可以先寻找一个插槽上空着的堆
		if (SearchFreeStack(Item) == Slots.Num()) {
			//插槽上没有空的位置了
			if (SearchEmptySlot() != Slots.Num()) {
				FoundIndex = SearchEmptySlot();
				//有完全空的插槽
				if (Amount > MaxStackSize)
				{//存不下
					Slots[FoundIndex] = FInventorySlot{ Item,MaxStackSize };
					UpdateSlotByIndex(FoundIndex);
					return AddItem(Item, Amount - MaxStackSize);//继续存
				}
				else {
					Slots[FoundIndex] = FInventorySlot{ Item,Amount };
					UpdateSlotByIndex(FoundIndex);
					return 0;
				}
			}
			else {
				return Amount;
			}
		}
		else {
			//该插槽上还有空的地方
			FoundIndex = SearchFreeStack(Item);
			if ((Slots[FoundIndex].Amount + Amount) > MaxStackSize) {
				int TempAmout = (Slots[FoundIndex].Amount + Amount) - MaxStackSize;
				//插槽当前已经存放的数量+要存放的数量是否大于插槽最大能够存放的数量
				Slots[FoundIndex] = FInventorySlot{ Item,MaxStackSize };
				UpdateSlotByIndex(FoundIndex);
				return AddItem(Item, TempAmout);
			}
			else {
				Slots[FoundIndex] = FInventorySlot{ Item,(Slots[FoundIndex].Amount + Amount) };
				UpdateSlotByIndex(FoundIndex);
				return 0;
			}
		}
	}
	else {
		//不能被堆积，只能找空插槽，一个插槽存放一个物品
		if (SearchEmptySlot() != Slots.Num()) {
			//找到了空插槽
			FoundIndex = SearchEmptySlot();
			Slots[FoundIndex] = FInventorySlot{ Item,1 };
			UpdateSlotByIndex(FoundIndex);
			if (Amount > 1) {
				//是否一种物品都已经存放完毕了，10个物品，不是一下子存放是个，而是存放10次1个			
				return AddItem(Item, Amount - 1);
			}
			else {
				return 0;
			}
		}
		else {
			return Amount;
		}
	}
}

bool AStorage::BIsSlotEmpty(int Index)
{
	if (Slots[Index].ItemClass) {
		return false;
	}
	else {
		return true;
	}
}

class ABaseItem* AStorage::GetItemAtIndex(int Index)
{
	if (!BIsSlotEmpty(Index)) {
		return Slots[Index].ItemClass->GetDefaultObject<ABaseItem>();
	}
	else {
		return nullptr;
	}
}

void AStorage::UpdateSlotByIndex(int Index)
{
	StorageWidget->StorageSlotWidgets[Index]->UpdateSlot();
}

int AStorage::GetAmountAtIndex(int Index)
{
	return Slots[Index].Amount;
}

bool AStorage::SwapSlot(int Index1, int Index2)
{
	if (Index1 >= Slots.Num() || Index2 >= Slots.Num()) {
		return false;
	}
	else {
		FInventorySlot TempSlot = Slots[Index1];
		Slots[Index1] = Slots[Index2];
		Slots[Index2] = TempSlot;
		UpdateSlotByIndex(Index1);
		UpdateSlotByIndex(Index2);
		return true;
	}
}

bool AStorage::AddToIndex(int FromIndex, int ToIndex)
{
	if (Slots[FromIndex].ItemClass == Slots[ToIndex].ItemClass&&GetItemAtIndex(FromIndex)->ItemInfo.bCanBeStacked&&GetAmountAtIndex(ToIndex) < MaxStackSize) {
		if ((GetAmountAtIndex(FromIndex) + GetAmountAtIndex(ToIndex)) < MaxStackSize) {
			Slots[ToIndex] = FInventorySlot{ Slots[FromIndex].ItemClass,GetAmountAtIndex(FromIndex) + GetAmountAtIndex(ToIndex) };
			Slots[FromIndex] = FInventorySlot{ nullptr,0 };

		}
		else {
			Slots[FromIndex] = FInventorySlot{ Slots[FromIndex].ItemClass,GetAmountAtIndex(FromIndex) + GetAmountAtIndex(ToIndex) - MaxStackSize };
			Slots[ToIndex] = FInventorySlot{ Slots[FromIndex].ItemClass,MaxStackSize };

		}
		UpdateSlotByIndex(FromIndex);
		UpdateSlotByIndex(ToIndex);
		return true;
	}
	else {
		return false;
	}
}


bool AStorage::RemoveItemAtIndex(int Index, int Amount)
{
	if (!BIsSlotEmpty(Index) && Amount > 0) {
		if (Amount >= GetAmountAtIndex(Index)) {	
			Slots[Index] = FInventorySlot{ nullptr,0 };
			UpdateSlotByIndex(Index);
			return true;
		}
		else {
			Slots[Index] = FInventorySlot{ Slots[Index].ItemClass,GetAmountAtIndex(Index) - Amount };
			UpdateSlotByIndex(Index);
			return true;
		}
	}
	else {
		return false;
	}
}

bool AStorage::AddItemToIndex(int Index, TSubclassOf<ABaseItem> Item, int Amount)
{
	if (BIsSlotEmpty(Index) && Amount < MaxStackSize) {
		Slots[Index] = FInventorySlot{ Item,Amount };
		UpdateSlotByIndex(Index);
		return true;
	}
	else {
		return false;
	}
}

bool AStorage::IncreaseAmountAtIndex(int Index, int Amount)
{
	if (!BIsSlotEmpty(Index) && (GetAmountAtIndex(Index) + Amount) <= MaxStackSize) {
		Slots[Index].Amount += Amount;
		UpdateSlotByIndex(Index);
		return true;
	}
	else {
		return false;
	}
}

void AStorage::OpenStorage()
{
	bStorageOpen = true;
	StorageWidget->SetVisibility(ESlateVisibility::Visible);
}

void AStorage::CloseStorage()
{
	bStorageOpen = false;
	StorageWidget->SetVisibility(ESlateVisibility::Hidden);
}

bool AStorage::MoveFromInvToStorageByIndex(class AInventory* Inventory, int InvetoryIndex, int StorageIndex)
{
	int AmountToAdd;
	if (BIsSlotEmpty(StorageIndex)) {
		AmountToAdd=Inventory->GetAmountAtIndex(InvetoryIndex);
		if (AddItemToIndex(StorageIndex, Inventory->Slots[InvetoryIndex].ItemClass, AmountToAdd)) {
			Inventory->RemoveItemAtIndex(InvetoryIndex, AmountToAdd);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (Slots[StorageIndex].ItemClass == Inventory->Slots[InvetoryIndex].ItemClass&&GetItemAtIndex(StorageIndex)->ItemInfo.bCanBeStacked&&GetAmountAtIndex(StorageIndex) < MaxStackSize) {
			AmountToAdd = (MaxStackSize - GetAmountAtIndex(StorageIndex) < Inventory->GetAmountAtIndex(InvetoryIndex))
				? (MaxStackSize - GetAmountAtIndex(StorageIndex))
				: Inventory->GetAmountAtIndex(InvetoryIndex);
			if (IncreaseAmountAtIndex(StorageIndex, AmountToAdd)) {
				Inventory->RemoveItemAtIndex(InvetoryIndex, AmountToAdd);
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
}

void AStorage::SaveStorage()
{
	if(!StorageSaveInstance)
	StorageSaveInstance= Cast<UStorageSave>(UGameplayStatics::CreateSaveGameObject(UStorageSave::StaticClass()));

	StorageSaveInstance->SavedSlots = Slots;

	UGameplayStatics::SaveGameToSlot(StorageSaveInstance, SavedSlotName, 0);
}

void AStorage::LoadStorage()
{
	if (!StorageSaveInstance) {
		StorageSaveInstance =Cast<UStorageSave>(UGameplayStatics::LoadGameFromSlot(SavedSlotName, 0)) ;
	}

	Slots = StorageSaveInstance->SavedSlots;
}


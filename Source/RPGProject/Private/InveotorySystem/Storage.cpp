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
	//�ȸ�����Ʒ�����������ж�
	if (Item->GetDefaultObject<ABaseItem>()->ItemInfo.bCanBeStacked) {
		//����Ʒ���Ա��ѻ�����ʱ������Ѱ��һ������Ͽ��ŵĶ�
		if (SearchFreeStack(Item) == Slots.Num()) {
			//�����û�пյ�λ����
			if (SearchEmptySlot() != Slots.Num()) {
				FoundIndex = SearchEmptySlot();
				//����ȫ�յĲ��
				if (Amount > MaxStackSize)
				{//�治��
					Slots[FoundIndex] = FInventorySlot{ Item,MaxStackSize };
					UpdateSlotByIndex(FoundIndex);
					return AddItem(Item, Amount - MaxStackSize);//������
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
			//�ò���ϻ��пյĵط�
			FoundIndex = SearchFreeStack(Item);
			if ((Slots[FoundIndex].Amount + Amount) > MaxStackSize) {
				int TempAmout = (Slots[FoundIndex].Amount + Amount) - MaxStackSize;
				//��۵�ǰ�Ѿ���ŵ�����+Ҫ��ŵ������Ƿ���ڲ������ܹ���ŵ�����
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
		//���ܱ��ѻ���ֻ���ҿղ�ۣ�һ����۴��һ����Ʒ
		if (SearchEmptySlot() != Slots.Num()) {
			//�ҵ��˿ղ��
			FoundIndex = SearchEmptySlot();
			Slots[FoundIndex] = FInventorySlot{ Item,1 };
			UpdateSlotByIndex(FoundIndex);
			if (Amount > 1) {
				//�Ƿ�һ����Ʒ���Ѿ��������ˣ�10����Ʒ������һ���Ӵ���Ǹ������Ǵ��10��1��			
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


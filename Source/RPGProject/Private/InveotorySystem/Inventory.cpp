// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/InveotorySystem/Inventory.h"
#include "Public/InveotorySystem/BaseItem.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Public/UserWidget/Inventory/InventoryWidget.h"
#include "Public/UserWidget/Inventory/InventorySlotWidget.h"
#include "Public/UserWidget/Inventory/ActionMenuWidget.h"
#include "Components/BoxComponent.h"
#include "Public/UserWidget/Inventory/CraftingMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Public/SaveGames/InventorySave.h"
#include "Public/InveotorySystem/Storage.h"
#include "Public/UserWidget/Inventory/ItemHotkey.h"
#include "Public/UserWidget/Inventory/ItemHotkeyWidget.h"

// Sets default values
AInventory::AInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ARBaseCharacter>(GetOwner());

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) {
		LoadInventory();
		if (CurrentWeight > TotalWeight) {
			bOverload = true;
			PlayerCharacter->OnOverloaded();
		}
	}

	Slots.SetNum(AmountOfSlots);

}

bool AInventory::BIsSlotEmpty(int Index)
{
	if (Slots[Index].ItemClass) {
		return false;
	}
	else {
		return true;
	}
}

class ABaseItem* AInventory::GetItemAtIndex(int Index)
{
	if (!BIsSlotEmpty(Index)) {
		return Slots[Index].ItemClass->GetDefaultObject<ABaseItem>();
	}
	else {
		return nullptr;
	}
}

void AInventory::UpdateSlotByIndex(int Index)
{
	PlayerCharacter->MainUserWidget->InventoryWidget->InventorySlotWidgets[Index]->UpdateSlot();
	UpdateHotkeyByIndex(Index);
}

int AInventory::GetAmountAtIndex(int Index)
{
	return Slots[Index].Amount;
}

bool AInventory::RemoveItemAtIndex(int Index, int Amount)
{
	if (!BIsSlotEmpty(Index) && Amount > 0) {
		if (Amount >= GetAmountAtIndex(Index)) {
			RemoveWeightForItem(Slots[Index].ItemClass, GetAmountAtIndex(Index));
			Slots[Index] = FInventorySlot{ nullptr,0 };
			UpdateSlotByIndex(Index);
			UpdateCraftingMenu();
			return true;
		}
		else {
			Slots[Index] = FInventorySlot{ Slots[Index].ItemClass,GetAmountAtIndex(Index) - Amount };
			RemoveWeightForItem(Slots[Index].ItemClass, Amount);
			UpdateSlotByIndex(Index);
			UpdateCraftingMenu();
			return true;
		}
	}
	else {
		return false;
	}
}

bool AInventory::SwapSlot(int Index1, int Index2)
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
		HandleSwapForHotkeys(Index1, Index2);
		return true;
	}
}

bool AInventory::SplitStack(int Index, int Amout)
{
	if (!BIsSlotEmpty(Index)) {
		if (GetAmountAtIndex(Index) > Amout&&GetItemAtIndex(Index)->ItemInfo.bCanBeStacked) {
			int FoudIndex = SearchEmptySlot();
			if ( FoudIndex!= Slots.Num()) {
				Slots[Index] = FInventorySlot{ Slots[Index].ItemClass,GetAmountAtIndex(Index )-Amout };
				Slots[FoudIndex] = FInventorySlot{ Slots[Index].ItemClass,Amout };
				UpdateSlotByIndex(Index);
				UpdateSlotByIndex(FoudIndex);
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
	else {
		return false;
	}
}

void AInventory::UseItemAtIndex(int Index)
{
	if (Slots[Index].ItemClass) {
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABaseItem* Item= GetWorld()->SpawnActor<ABaseItem>(Slots[Index].ItemClass,Params);
		Item->InventoryRef = this;
		Item->Index = Index;
		if (Item->ItemInfo.Category == EItemCategories::Equipment) {
			Item->BoxCollisonComp->SetGenerateOverlapEvents(false);
		}
		Item->OnUsed();
	}
}

void AInventory::UpdateActionMenuPosition(class UInventorySlotWidget* Slot)
{
	PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->Update(Slot->SlotIndex);
	int Y=Slot->SlotIndex / 7;
	int X = Slot->SlotIndex % 7;
	PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->SetRenderTranslation(FVector2D(X*93,Y*93));
	PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->SetVisibility(ESlateVisibility::Visible);
}

bool AInventory::AddToIndex(int FromIndex, int ToIndex)
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

bool AInventory::SplitStackToIndex(int FromIndex, int ToIndex, int Amount)
{
	if (!BIsSlotEmpty(FromIndex) && BIsSlotEmpty(ToIndex)
		&& GetItemAtIndex(FromIndex)->ItemInfo.bCanBeStacked
		&&GetAmountAtIndex(FromIndex) > 1 && GetAmountAtIndex(FromIndex) > Amount) {
		Slots[FromIndex] = FInventorySlot{ Slots[FromIndex].ItemClass,GetAmountAtIndex(FromIndex) - Amount };
		Slots[ToIndex] = FInventorySlot{ Slots[FromIndex].ItemClass,Amount };
		UpdateSlotByIndex(FromIndex);
		UpdateSlotByIndex(ToIndex);
		return true;
	}
	else {
		return false;
	}
}

void AInventory::SortInventory(ESortType Type, bool BReversed)
{
	if (FilterEmptySlot().Num() > 1) {
		Slots = BubbleSortArray(FilterEmptySlot(), Type, BReversed);
		Slots.SetNum(AmountOfSlots);
		for (int i = 0; i < Slots.Num(); i++) {
			UpdateSlotByIndex(i);
		}
	}
	else {
		Slots = FilterEmptySlot();
		Slots.SetNum(AmountOfSlots);
		for (int i = 0; i < Slots.Num(); i++) {
			UpdateSlotByIndex(i);
		}
	}
}

TArray<FInventorySlot> AInventory::FilterEmptySlot()
{
	TArray<FInventorySlot> TempSlots;
	for (FInventorySlot Slot : Slots) {
		if (Slot.ItemClass) {
			TempSlots.Add(Slot);
		}
	}
	return TempSlots;
}

TArray<FInventorySlot> AInventory::BubbleSortArray(TArray<FInventorySlot> InArray, ESortType Type, bool BReversed)
{
	TArray<FInventorySlot> TempSlots = InArray;
	for (int End = TempSlots.Num() - 1; End > 0; End--) {
		for (int i = 0; i < End; i++) {
			switch (Type)
			{
			case ESortType::Category:
				if (!BReversed) {
					if (TempSlots[i].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Category > TempSlots[i + 1].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Category) {
						FInventorySlot TempSlot = TempSlots[i];
						TempSlots[i] = TempSlots[i + 1];
						TempSlots[i + 1] = TempSlot;
					}
				}
				else {
					if (TempSlots[i].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Category < TempSlots[i + 1].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Category) {
						FInventorySlot TempSlot = TempSlots[i];
						TempSlots[i] = TempSlots[i + 1];
						TempSlots[i + 1] = TempSlot;
					}
				}
				break;
			case ESortType::Amount:
				if (!BReversed)
				{
					if (TempSlots[i].Amount > TempSlots[i + 1].Amount) {
						FInventorySlot TempSlot = TempSlots[i];
						TempSlots[i] = TempSlots[i + 1];
						TempSlots[i + 1] = TempSlot;
					}
				}
				else {
					if (TempSlots[i].Amount < TempSlots[i + 1].Amount) {
						FInventorySlot TempSlot = TempSlots[i];
						TempSlots[i] = TempSlots[i + 1];
						TempSlots[i + 1] = TempSlot;
					}

				}
				break;
			case ESortType::Name:
				if (!BReversed) {
					if (TempSlots[i].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Name.ToString().Compare(TempSlots[i + 1].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Name.ToString())>0) {
						FInventorySlot TempSlot = TempSlots[i];
						TempSlots[i] = TempSlots[i + 1];
						TempSlots[i + 1] = TempSlot;
					}
				}
				else {
					if (TempSlots[i].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Name.ToString().Compare(TempSlots[i + 1].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Name.ToString()) < 0) {
						FInventorySlot TempSlot = TempSlots[i];
						TempSlots[i] = TempSlots[i + 1];
						TempSlots[i + 1] = TempSlot;
					}
				}
				break;
			default:
				break;
			}
		}
	}
	return TempSlots;
}

void AInventory::UpdateWeight()
{
	PlayerCharacter->MainUserWidget->InventoryWidget->UpdateWeight(CurrentWeight, TotalWeight);
}

void AInventory::AddWeight(float Weight)
{
	CurrentWeight += Weight;
	UpdateWeight();
	if (!bOverload&&CurrentWeight > TotalWeight) {
		bOverload = true;
		PlayerCharacter->OnOverloaded();
	}
}

void AInventory::RemoveWeight(float Weight)
{
	CurrentWeight -= Weight;
	UpdateWeight();
	if (bOverload&&CurrentWeight <= TotalWeight) {
		bOverload = false;
		PlayerCharacter->OnOverloadEnd();
	}
}

void AInventory::AddWeightForItem(TSubclassOf<ABaseItem> Item, int Amount)
{
	AddWeight(Item->GetDefaultObject<ABaseItem>()->ItemInfo.Weight*Amount);
}

void AInventory::RemoveWeightForItem(TSubclassOf<ABaseItem> Item, int Amount)
{
	RemoveWeight(Item->GetDefaultObject<ABaseItem>()->ItemInfo.Weight*Amount);
}

int AInventory::GetTotalAmountOfItem(TSubclassOf<ABaseItem> Item, TArray<int> &ItemIndices)
{
	int TempAmount = 0;
	for (int i = 0; i < Slots.Num(); i++) {
		if (Slots[i].ItemClass) {
			if (Slots[i].ItemClass == Item) {
				TempAmount += Slots[i].Amount;
				ItemIndices.Add(i);
			}
		}
	}
	return TempAmount;
}

bool AInventory::RemoveItem(TSubclassOf<ABaseItem> Item, int Amount)
{
	TArray<int> ItemIndices;
	if (GetTotalAmountOfItem(Item, ItemIndices) >= Amount) {
		for (int Index : ItemIndices) {
			if (GetAmountAtIndex(Index) >= Amount) {
				RemoveItemAtIndex(Index, Amount);
				break;
			}
			else {
				Amount -=GetAmountAtIndex(Index);
				RemoveItemAtIndex(Index, GetAmountAtIndex(Index));
			}
		}
		return true;
	}
	else {
		return false;
	}
}

void AInventory::UpdateCraftingMenu()
{
	if(PlayerCharacter->MainUserWidget->CraftingMenu->CurrentItem)
	PlayerCharacter->MainUserWidget->CraftingMenu->UpdateDetailWindow(PlayerCharacter->MainUserWidget->CraftingMenu->CurrentItem);
}

void AInventory::AddToLootedPickups(FSavedPickup Pickup)
{
	LootedPickups.Add(Pickup);
}

void AInventory::SaveInventory()
{
	if (!InventorySaveInstance) {
		InventorySaveInstance =Cast<UInventorySave>(UGameplayStatics::CreateSaveGameObject(UInventorySave::StaticClass())) ;
	}

	InventorySaveInstance->SavedWeight = CurrentWeight;
	InventorySaveInstance->SavedSlots = Slots;
	InventorySaveInstance->SavedPickups = LootedPickups;

	UGameplayStatics::SaveGameToSlot(InventorySaveInstance, SaveSlotName, 0);
}

bool AInventory::BContainsID(int InID, int &Rest)
{
	for (FSavedPickup Pickup:LootedPickups)
	{
		if (Pickup.ID == InID) {
			Rest = Pickup.RestAmount;
			return true;
		}
	}
	return false;
}

void AInventory::LoadPickups()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseItem::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors) {
		ABaseItem* Item= Cast<ABaseItem>(Actor);
		if (Item) {
			if (Item->ID != -1) {
				int Rest;
				if (BContainsID(Item->ID, Rest)) {
					if (Rest > 0) {
						Item->Amount = Rest;
					}
					else {
						Item->Destroy();
					}
				}
			}
		}
	}
}

void AInventory::LoadInventory()
{
	if(!InventorySaveInstance)
	InventorySaveInstance=Cast<UInventorySave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)) ;

	CurrentWeight = InventorySaveInstance->SavedWeight;
	Slots = InventorySaveInstance->SavedSlots;
	LootedPickups = InventorySaveInstance->SavedPickups;

	LoadPickups();
}

bool AInventory::AddItemToIndex(int Index, TSubclassOf<ABaseItem> Item, int Amount)
{
	if (BIsSlotEmpty(Index) && Amount < MaxStackSize) {
		Slots[Index] = FInventorySlot{ Item,Amount };
		AddWeightForItem(Item, Amount);
		UpdateSlotByIndex(Index);
		return true;
	}
	else {
		return false;
	}
}

bool AInventory::IncreaseAmountAtIndex(int Index, int Amount)
{
	if (!BIsSlotEmpty(Index) && (GetAmountAtIndex(Index) + Amount) <= MaxStackSize) {
		Slots[Index].Amount += Amount;
		AddWeightForItem(Slots[Index].ItemClass, Slots[Index].Amount);
		UpdateSlotByIndex(Index);
		return true;
	}
	else {
		return false;
	}
}

bool AInventory::MoveFromStorageToInvByIndex(class AStorage* Storage, int StorageIndex, int InventoryIndex)
{
	int AmountToAdd;
	if (BIsSlotEmpty(InventoryIndex)) {
		AmountToAdd = Storage->GetAmountAtIndex(StorageIndex);
		if (AddItemToIndex(InventoryIndex, Storage->Slots[StorageIndex].ItemClass, AmountToAdd)) {
			Storage->RemoveItemAtIndex(StorageIndex, AmountToAdd);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (Slots[InventoryIndex].ItemClass == Storage->Slots[StorageIndex].ItemClass&&GetItemAtIndex(InventoryIndex)->ItemInfo.bCanBeStacked&&GetAmountAtIndex(InventoryIndex) < MaxStackSize) {
			AmountToAdd = (MaxStackSize - GetAmountAtIndex(InventoryIndex) < Storage->GetAmountAtIndex(StorageIndex))
				? (MaxStackSize - GetAmountAtIndex(InventoryIndex))
				: Storage->GetAmountAtIndex(StorageIndex);
			if (IncreaseAmountAtIndex(InventoryIndex, AmountToAdd)) {
				Storage->RemoveItemAtIndex(StorageIndex, AmountToAdd);
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

void AInventory::UpdateHotkeyByIndex(int Index)
{
	for (UItemHotkey* Hotkey: PlayerCharacter->MainUserWidget->ItemHotkeyWidgets) {
		if (!Hotkey->ItemHotkeySlot->bEmpty&&Hotkey->ItemHotkeySlot->InventoryIndex==Index) {
			Hotkey->ItemHotkeySlot->Update();
		}
	}
}

void AInventory::HandleItemHotkeyPress(FKey Key)
{
	for (UItemHotkey* Hotkey:PlayerCharacter->MainUserWidget->ItemHotkeyWidgets)
	{
		if (Hotkey->Key == Key&&!Hotkey->ItemHotkeySlot->bEmpty) {
			UseItemAtIndex(Hotkey->ItemHotkeySlot->InventoryIndex);
			break;
		}
	}
}

void AInventory::HandleSwapForHotkeys(int Index1, int Index2)
{
	for (UItemHotkey* Hotkey : PlayerCharacter->MainUserWidget->ItemHotkeyWidgets) {
		if (!Hotkey->ItemHotkeySlot->bEmpty) {
			if (Hotkey->ItemHotkeySlot->InventoryIndex == Index1) {
				Hotkey->ItemHotkeySlot->InventoryIndex = Index2;
			}
			else if (Hotkey->ItemHotkeySlot->InventoryIndex == Index2) {
				Hotkey->ItemHotkeySlot->InventoryIndex = Index1;
			}
		}
	}
}

int AInventory::SearchEmptySlot()
{
	for (int i = 0; i < Slots.Num(); i++) {
		if (!Slots[i].ItemClass) {
			return i;
		}
	}
	return Slots.Num();
}

int AInventory::SearchFreeStack(TSubclassOf<class ABaseItem> Item)
{
	for (int i = 0; i < Slots.Num(); i++) {
		if (Slots[i].ItemClass&&Slots[i].ItemClass == Item && Slots[i].Amount < MaxStackSize) {
			return i;
		}
	}
	return Slots.Num();
}

// Called every frame
void AInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//���ص������֮��ʣ�µ���Ʒ������
int AInventory::AddItem(TSubclassOf<class ABaseItem> Item, int Amount)
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
					AddWeightForItem(Item, MaxStackSize);
					UpdateCraftingMenu();
					UpdateSlotByIndex(FoundIndex);				
					return AddItem(Item, Amount - MaxStackSize);//������
				}
				else {
					Slots[FoundIndex] = FInventorySlot{ Item,Amount };
					AddWeightForItem(Item, Amount);
					UpdateCraftingMenu();
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
				AddWeightForItem(Item, MaxStackSize);
				UpdateCraftingMenu();
				UpdateSlotByIndex(FoundIndex);
				return AddItem(Item, TempAmout);
			}
			else {
				Slots[FoundIndex] = FInventorySlot{ Item,(Slots[FoundIndex].Amount + Amount) };
				AddWeightForItem(Item, Amount);
				UpdateCraftingMenu();
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
			AddWeightForItem(Item, 1);
			UpdateCraftingMenu();
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


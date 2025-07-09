// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "Inventory.generated.h"

UCLASS()
class RPGPROJECT_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();

protected:

	int AmountOfSlots=42;//插槽的数量

	int MaxStackSize=99;//一个插槽最多存放的数量

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int SearchEmptySlot();

	int SearchFreeStack(TSubclassOf<class ABaseItem> Item);

public:	

	class ARBaseCharacter* PlayerCharacter;
	TArray<FInventorySlot> Slots;

	float CurrentWeight;

	UPROPERTY(EditAnywhere,Category=InventoryInfo)
	float TotalWeight=250;

	bool bOverload;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	int AddItem(TSubclassOf<class ABaseItem> Item,int Amount);

	bool BIsSlotEmpty(int Index);

	class ABaseItem* GetItemAtIndex(int Index);

	void UpdateSlotByIndex(int Index);

	int GetAmountAtIndex(int Index);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemAtIndex(int Index,int Amount);
	UFUNCTION(BlueprintCallable)
	bool SwapSlot(int Index1,int Index2);
	UFUNCTION(BlueprintCallable)
	bool SplitStack(int Index,int Amout);
	UFUNCTION(BlueprintCallable)
	void UseItemAtIndex(int Index);

	void UpdateActionMenuPosition(class UInventorySlotWidget* Slot);

	bool AddToIndex(int FromIndex,int ToIndex);

	bool SplitStackToIndex(int FromIndex,int ToIndex,int Amount);

	void SortInventory(ESortType Type,bool BReversed);

	TArray<FInventorySlot> FilterEmptySlot();

	TArray<FInventorySlot> BubbleSortArray(TArray<FInventorySlot> InArray,ESortType Type,bool BReversed);

	void UpdateWeight();

	void AddWeight(float Weight);

	void RemoveWeight(float Weight);

	void AddWeightForItem(TSubclassOf<ABaseItem> Item,int Amount);

	void RemoveWeightForItem(TSubclassOf<ABaseItem> Item, int Amount);

	int GetTotalAmountOfItem(TSubclassOf<ABaseItem> Item,TArray<int> &ItemIndices);

	bool RemoveItem(TSubclassOf<ABaseItem> Item,int Amount);

	UPROPERTY(EditAnywhere,Category=Item)
	TArray<TSubclassOf<ABaseItem>> CraftableItems;

	void UpdateCraftingMenu();

public:
	
	TArray<FSavedPickup> LootedPickups;
	void AddToLootedPickups(FSavedPickup Pickup);
	
	class UInventorySave* InventorySaveInstance;
	FString SaveSlotName = "InventorySave";

	void SaveInventory();

	bool BContainsID(int InID,int &Rest);

	void LoadPickups();

	void LoadInventory();

public:
	bool AddItemToIndex(int Index,TSubclassOf<ABaseItem> Item,int Amount);

	bool IncreaseAmountAtIndex(int Index,int Amount);

	bool MoveFromStorageToInvByIndex(class AStorage* Storage,int StorageIndex,int InventoryIndex);
public:
	void UpdateHotkeyByIndex(int Index);

	void HandleItemHotkeyPress(FKey Key);

	void HandleSwapForHotkeys(int Index1,int Index2);
};

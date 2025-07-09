// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "Storage.generated.h"

UCLASS()
class RPGPROJECT_API AStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStorage();

protected:

	int AmountOfSlots = 42;//插槽的数量

	int MaxStackSize = 99;//一个插槽最多存放的数量

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int SearchEmptySlot();

	int SearchFreeStack(TSubclassOf<class ABaseItem> Item);

public:

	class ARBaseCharacter* PlayerCharacter;
	TArray<FInventorySlot> Slots;
	class UStorageWidget* StorageWidget;
	bool bStorageOpen;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int AddItem(TSubclassOf<class ABaseItem> Item, int Amount);

	bool BIsSlotEmpty(int Index);

	class ABaseItem* GetItemAtIndex(int Index);

	void UpdateSlotByIndex(int Index);

	int GetAmountAtIndex(int Index);

	UFUNCTION(BlueprintCallable)
		bool SwapSlot(int Index1, int Index2);

	bool AddToIndex(int FromIndex, int ToIndex);
	UFUNCTION(BlueprintCallable)
		bool RemoveItemAtIndex(int Index, int Amount);

public:
	bool AddItemToIndex(int Index, TSubclassOf<ABaseItem> Item, int Amount);

	bool IncreaseAmountAtIndex(int Index, int Amount);

	void OpenStorage();

	void CloseStorage();

	bool MoveFromInvToStorageByIndex(class AInventory* Inventory,int InvetoryIndex,int StorageIndex);

	class UStorageSave* StorageSaveInstance;
	FString SavedSlotName = "StorageSave";

	void SaveStorage();

	void LoadStorage();
};

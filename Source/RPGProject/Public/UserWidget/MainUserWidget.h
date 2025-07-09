// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/UserWidget/SkillHotkey.h"
#include "Queue.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "MainUserWidget.generated.h"

class UProgressBar;
class	UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()

	UTextBlock* LevelText;
	
	 UProgressBar* HPProgressBar;

	 UProgressBar* MPProgressBar;

	 UPROPERTY(Meta=(BindWidget))
	 class UVerticalBox* HotkeyRowContainer;

	 TArray<class UHotkeyRow*> HotkeyRows;

	 UPROPERTY(Meta = (BindWidget))
	 UTextBlock* CurrentExpText;
	 UPROPERTY(Meta = (BindWidget))
	 UTextBlock* NeededExpToNextLevelText;
	 UPROPERTY(Meta = (BindWidget))
	 UProgressBar* ExpBar;
	 UPROPERTY(Meta = (BindWidget))
	 class USizeBox* QuestListSizeBox;
	 UPROPERTY(Meta = (BindWidget))
	 class UScrollBox* QuestList;
	 UPROPERTY(Meta = (BindWidget))
	 UButton* Button_Quest;
	 UPROPERTY(Meta = (BindWidget))
	 UButton* Button_Skill;

	 UPROPERTY(Meta = (BindWidget))
	UButton* Button_Inventory;
	 UPROPERTY(Meta = (BindWidget))
	 UButton* Button_Setting;
	 UPROPERTY(Meta = (BindWidget))
	 class USettingWidget* SettingWidget;

	 virtual bool Initialize() override;

	 virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
public:
	UPROPERTY(Meta = (BindWidget))
		class UQuestJournal* QuestJournal;

	UPROPERTY(BlueprintReadWrite)
	bool bOutSide = true;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UQuestWidget*> QuestWidgets;
	class AQuestManager* QuestManager;

	UPROPERTY(Meta = (BindWidget))
		class UMainTreeWidget* SkillTree;

	UPROPERTY(BlueprintReadOnly)
		TArray<FKey> Keys;
	UPROPERTY(BlueprintReadOnly)
		TArray<USkillHotkey*> AllHotkeySlots;

	UPROPERTY(Meta = (BindWidget))
		UTextBlock* EnemyNameLevel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* EnemyHpText;
	UPROPERTY(Meta = (BindWidget))
		UProgressBar* EnemyHpBar;
	UPROPERTY(Meta = (BindWidget))
	class UBorder* EnemyBorder;
	UPROPERTY(Meta = (BindWidget))
	class UHorizontalBox* BuffBox;

	UPROPERTY(Meta = (BindWidget))
	class UMinimapWidget* Minimap;
	UPROPERTY(Meta = (BindWidget))
	class UBorder* ObtainContainer;

	UPROPERTY(Meta = (BindWidget))
		class UBorder* ReadableContainer;

	void SetLevelText(FText Text);

	void SetHPProgressBar(float Percent);

	void SetMPProgressBar(float Percent);

	void GenerateHotkeys(TArray<FKey> KeysToGenerate,int KeysPerRow);

	FORCEINLINE TArray<USkillHotkey*> GetAllHotkeySlots() { return AllHotkeySlots; }


	UFUNCTION(BlueprintNativeEvent,Category=Animation)
	void PlayPopupAnimation();

	class UQuestWidget* AddQuestToList(class ABaseQuest* Quest);

	void PlayPopupAnimation_Implementation();

	void SetExpPanel(float CurrentExp,float NeededExpToNextLevel);

	UFUNCTION()
		void OnQuestButtonClicked();

	UFUNCTION()
		void OnSkillButtonClicked();
	UFUNCTION()
	void OnInventoryButtonClicked();
	UFUNCTION()
	void OnSettingButtonClicked();

	bool bQuestVisibling = false;
	bool bTreeShown=false;
	bool bInventoryVisibling = false;
	bool bSettingVisibling = false;
public:
	UPROPERTY(Meta=(BindWidget))
	class UInventoryWidget* InventoryWidget;
	UPROPERTY(Meta = (BindWidget))
	class UThrowWidget* ThrowWidget;
	UPROPERTY(BlueprintReadOnly,Meta = (BindWidget))
	class UCraftingMenuWidget* CraftingMenu;
	
		class UShopWidget* ShopWidget;

	TQueue<FInventorySlot> ObtainedItemQueue;

	void AddItemToObtainedQueue(TSubclassOf<class ABaseItem> ItemClass,int Amount);

	UFUNCTION(BlueprintCallable)
	void OnObtainMessageEnd();

public:
	UPROPERTY(Meta = (BindWidget))
	class UStorageWidget* StorageWidget;

public:
	class AInventory* Inventory;

	UPROPERTY(Meta = (BindWidget))
	class UHorizontalBox* ItemHotkeyBar;

	TArray<class UItemHotkey*> ItemHotkeyWidgets;

	void GenerateItemHotkeys(TArray<FKey> ItemKeysToGenerate);
};

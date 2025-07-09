// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/UserWidget/MainUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Public/UserWidget/HotkeyRow.h"
#include "Components/VerticalBox.h"
#include "Public/UserWidget/SkillDragOperation.h"
#include "Public/UserWidget/Quest/QuestWidget.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Public/UserWidget/Quest/QuestJournal.h"
#include "Components/Button.h"
#include "Public/UserWidget/SkillTree/MainTreeWidget.h"
#include "Public/UserWidget/Inventory/InventoryWidget.h"
#include "Public/UserWidget/Inventory/InventoryDragDropOperation.h"
#include "Public/UserWidget/Inventory/ItemDragDropOperation.h"
#include "Public/UserWidget/Inventory/InventorySlotWidget.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Public/UserWidget/Inventory/ThrowWidget.h"
#include "Components/WrapBox.h"
#include "Public/UserWidget/Inventory/ItemObtainWidget.h"
#include "Components/Border.h"
#include "Public/UserWidget/SettingWidget.h"
#include "Components/HorizontalBox.h"
#include "Public/UserWidget/Inventory/ItemHotkey.h"

bool UMainUserWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	LevelText = Cast<UTextBlock>(GetWidgetFromName("Text_Level"));
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName("ProgressBar_HP"));
	MPProgressBar = Cast<UProgressBar>(GetWidgetFromName("ProgressBar_MP"));


	Button_Quest->OnClicked.AddDynamic(this, &UMainUserWidget::OnQuestButtonClicked);
	Button_Skill->OnClicked.AddDynamic(this, &UMainUserWidget::OnSkillButtonClicked);
	Button_Inventory->OnClicked.AddDynamic(this, &UMainUserWidget::OnInventoryButtonClicked);
	Button_Setting->OnClicked.AddDynamic(this, &UMainUserWidget::OnSettingButtonClicked);
	return true;
}

bool UMainUserWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	//看似把技能丢在了地上，实际上丢在了主界面上
	if (Cast<USkillDragOperation>(InOperation))
	{
		if (Cast<USkillDragOperation>(InOperation)->FromHotkey)
			Cast<USkillDragOperation>(InOperation)->FromHotkey->ClearAssignedSpell();
		return true;
	}
	else if (Cast<UInventoryDragDropOperation>(InOperation))
	{
		Cast<UInventoryDragDropOperation>(InOperation)->WidgetToDrag->AddToViewport();
		Cast<UInventoryDragDropOperation>(InOperation)->WidgetToDrag->SetDesiredSizeInViewport(FVector2D(800, 600));

		Cast<UInventoryDragDropOperation>(InOperation)->WidgetToDrag->SetPositionInViewport(InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition())
			- Cast<UInventoryDragDropOperation>(InOperation)->MouseOffset, false
		);
		return true;
	}
	else if (Cast<UItemDragDropOperation>(InOperation))
	{
		UInventorySlotWidget* TempSlot = Cast<UItemDragDropOperation>(InOperation)->DraggedSlot;
		if (!TempSlot->ItemInfo.bCanBeStacked && TempSlot->Amount > 0)
		{
			InventoryWidget->InventoryRef->RemoveItemAtIndex(TempSlot->SlotIndex, 1);
		}
		else
		{
			ThrowWidget->Update(TempSlot->SlotIndex);
			ThrowWidget->SetVisibility(ESlateVisibility::Visible);
			InventoryWidget->InventoryBox->SetIsEnabled(false);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void UMainUserWidget::SetLevelText(FText Text)
{
	LevelText->SetText(Text);
}

void UMainUserWidget::SetHPProgressBar(float Percent)
{
	HPProgressBar->SetPercent(Percent);
}

void UMainUserWidget::SetMPProgressBar(float Percent)
{
	MPProgressBar->SetPercent(Percent);
}

void UMainUserWidget::GenerateHotkeys(TArray<FKey> KeysToGenerate, int KeysPerRow)
{
	TArray<FKey> LocalKeys = KeysToGenerate;
	TArray<FKey> LocalModifyKeys = KeysToGenerate;

	if (KeysToGenerate.Num() % KeysPerRow != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("The num of keys is invalid"));
		return;
	}

	if (LocalKeys.Num() == KeysPerRow)
	{
		UHotkeyRow* HotkeyRow = CreateWidget<UHotkeyRow>(GetWorld(), LoadClass<UHotkeyRow>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/WBP_HotkeyRow.WBP_HotkeyRow_C'")));
		HotkeyRow->SetHotkeys(LocalKeys);
		/*HotkeyRowContainer->AddChildToVerticalBox(HotkeyRow);*/
		AllHotkeySlots.Append(HotkeyRow->GenerateHotkeys());
		HotkeyRows.Add(HotkeyRow);
		for (int i = HotkeyRows.Num() - 1; i >= 0; i--)
		{
			HotkeyRowContainer->AddChildToVerticalBox(HotkeyRows[i]);
		}
	}
	else
	{
		LocalModifyKeys.SetNum(KeysPerRow);
		UHotkeyRow* HotkeyRow = CreateWidget<UHotkeyRow>(GetWorld(), LoadClass<UHotkeyRow>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/WBP_HotkeyRow.WBP_HotkeyRow_C'")));
		HotkeyRow->SetHotkeys(LocalModifyKeys);
		//HotkeyRowContainer->AddChildToVerticalBox(HotkeyRow);
		AllHotkeySlots.Append(HotkeyRow->GenerateHotkeys());
		HotkeyRows.Add(HotkeyRow);
		for (auto Key : LocalModifyKeys)
		{
			if (LocalKeys.Contains(Key))
			{
				LocalKeys.Remove(Key);
			}
		}
		GenerateHotkeys(LocalKeys, KeysPerRow);
	}
}

class UQuestWidget* UMainUserWidget::AddQuestToList(class ABaseQuest* Quest)
{
	if (Quest)
	{
		UQuestWidget* QuestWidget = CreateWidget<UQuestWidget>(GetWorld(), LoadClass<UQuestWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Quest/WBP_Quest.WBP_Quest_C'")));
		QuestWidget->QuestManager = QuestManager;
		QuestWidget->AssignedQuest = Quest;
		QuestWidgets.Add(QuestWidget);

		UScrollBoxSlot* Temp = Cast<UScrollBoxSlot>(QuestList->AddChild(QuestWidget));
		Temp->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		Temp->SetPadding(FMargin(0, 8, 0, 0));
		return QuestWidget;
	}
	else
	{
		return nullptr;
	}
}

void UMainUserWidget::PlayPopupAnimation_Implementation()
{

}

void UMainUserWidget::SetExpPanel(float CurrentExp, float NeededExpToNextLevel)
{
	ExpBar->SetPercent(CurrentExp / NeededExpToNextLevel);
	CurrentExpText->SetText(FText::AsNumber(CurrentExp));
	NeededExpToNextLevelText->SetText(FText::AsNumber(NeededExpToNextLevel));
}

void UMainUserWidget::OnQuestButtonClicked()
{
	if (bQuestVisibling)
	{
		QuestJournal->SetVisibility(ESlateVisibility::Hidden);
		bQuestVisibling = false;
	}
	else
	{
		QuestJournal->SetVisibility(ESlateVisibility::Visible);
		bQuestVisibling = true;
	}
}

void UMainUserWidget::OnSkillButtonClicked()
{
	if (bTreeShown)
	{
		SkillTree->SetVisibility(ESlateVisibility::Hidden);
		bTreeShown = false;
	}
	else
	{
		SkillTree->SetVisibility(ESlateVisibility::Visible);
		bTreeShown = true;
	}
}

void UMainUserWidget::OnInventoryButtonClicked()
{
	if (bInventoryVisibling)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		bInventoryVisibling = false;
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		bInventoryVisibling = true;
	}
}

void UMainUserWidget::OnSettingButtonClicked()
{
	if (bSettingVisibling)
	{
		SettingWidget->SetVisibility(ESlateVisibility::Hidden);
		bSettingVisibling = false;
	}
	else
	{
		SettingWidget->SetVisibility(ESlateVisibility::Visible);
		bSettingVisibling = true;
	}
}

void UMainUserWidget::AddItemToObtainedQueue(TSubclassOf<class ABaseItem> ItemClass, int Amount)
{
	if (!ObtainedItemQueue.IsEmpty())
	{
		ObtainedItemQueue.Enqueue(FInventorySlot{ ItemClass,Amount });
	}
	else
	{

		UItemObtainWidget* ObtainWidget = CreateWidget<UItemObtainWidget>(GetWorld(), LoadClass<UItemObtainWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_ItemObtain.WBP_ItemObtain_C'")));
		ObtainWidget->Init(ItemClass, Amount, this);
		ObtainContainer->AddChild(ObtainWidget);
		ObtainedItemQueue.Enqueue(FInventorySlot{ ItemClass,Amount });
	}
}

void UMainUserWidget::OnObtainMessageEnd()
{
	ObtainedItemQueue.Pop();
	ObtainContainer->ClearChildren();
	if (!ObtainedItemQueue.IsEmpty())
	{
		UItemObtainWidget* ObtainWidget = CreateWidget<UItemObtainWidget>(GetWorld(), LoadClass<UItemObtainWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_ItemObtain.WBP_ItemObtain_C'")));
		FInventorySlot TempSlot;

		ObtainedItemQueue.Peek(TempSlot);
		ObtainWidget->Init(TempSlot.ItemClass, TempSlot.Amount, this);
		ObtainContainer->AddChild(ObtainWidget);
	}
}

void UMainUserWidget::GenerateItemHotkeys(TArray<FKey> ItemKeysToGenerate)
{
	ItemHotkeyWidgets.Empty();
	ItemHotkeyBar->ClearChildren();
	for (FKey Key : ItemKeysToGenerate)
	{
		UItemHotkey* ItemHotkey = CreateWidget<UItemHotkey>(GetWorld(), LoadClass<UItemHotkey>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_ItemHotkey.WBP_ItemHotkey_C'")));
		ItemHotkey->Init(Key, Inventory);
		ItemHotkeyWidgets.Add(ItemHotkey);
		ItemHotkeyBar->AddChild(ItemHotkey);
	}
}

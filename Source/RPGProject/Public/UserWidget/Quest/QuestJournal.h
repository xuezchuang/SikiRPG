// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestJournal.generated.h"

class UTextBlock;
class UVerticalBox;
class UButton;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuestJournal : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta=(BindWidget))
	class UScrollBox* QuestList;
	UPROPERTY(Meta = (BindWidget))
	class UScrollBox* QuestDetailsBox;
	UPROPERTY(Meta = (BindWidget))
	class UQuestCategoryWidget* CatCurrent;
	UPROPERTY(Meta = (BindWidget))
	class UQuestCategoryWidget* CatCompleted;
	UPROPERTY(Meta = (BindWidget))
	class UQuestCategoryWidget* CatFailed;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* QuestName;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* QuestCategory;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* QuestRegion;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* SuggestedLevel;
	UPROPERTY(Meta = (BindWidget))
	UVerticalBox* RewardBox;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ExpReward;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* GoldReward;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Description;
	UPROPERTY(Meta = (BindWidget))
	UVerticalBox* QuestGoalBox;
	UPROPERTY(Meta = (BindWidget))
	UButton* SelectButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* CancelButton;
	UPROPERTY(Meta = (BindWidget))
	class UHorizontalBox* ButtonBox;
	UPROPERTY(Meta = (BindWidget))
	class UHorizontalBox* ExpRewardBox;
	UPROPERTY(Meta = (BindWidget))
	class UHorizontalBox* GoldRewardBox;

	virtual void NativeConstruct() override;
public:
	class ABaseQuest* SelectedQuest;
	class ARBaseCharacter* PlayerCharacter;

	class AQuestManager* QuestManager;

	TArray<class UQuestListEntry*> AllQuestListEntries;

	class UQuestListEntry* CurrentQuestListEntry;

	void UpdateSuggestedLevelColor();

	void UpdateDescription();

	void GenereateSubGoals();

	void UpdateDetailWindow();

	void AddQuestEntry(class ABaseQuest* Quest);

	void InitializeWithManager(class AQuestManager* QuestManager);

	void OnQuestClicked(class UQuestListEntry* ClickedQuestListEntry);

	UFUNCTION()
	void OnSelectButtonClicked();
};

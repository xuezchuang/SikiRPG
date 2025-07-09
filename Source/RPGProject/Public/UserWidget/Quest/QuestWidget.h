// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
		class UVerticalBox* SubGoalBox;
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* QuestName;
	class AQuestManager* QuestManager;
	class ABaseQuest* AssignedQuest;
	TArray<class USubGoalWidget*> SubGoalWidgets;
	class USubGoalWidget* SelectedSubGoalWidget;

	void GenerateSubWidgets();
	void UpdateQuest();
	bool BCurrentQuest();
	void SelectSubGoal(class USubGoalWidget* ClickedSubGoalWidget);

	void OnQuestSelected(class USubGoalWidget* SubGoalWidget);
};

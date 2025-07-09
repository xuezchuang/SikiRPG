// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/UserWidget/Quest/SubGoalWidget.h"
#include "Components/TextBlock.h"
#include "Public/Quest/BaseQuest.h"
#include "Public/UserWidget/Quest/QuestWidget.h"
#include "Public/Quest/QuestManager.h"
#include "Components/Button.h"

#define  LOCTEXT_NAMESPACE "SubGoalWidget"

void USubGoalWidget::OnSelectButtonClicked()
{
	if (QuestWidget->BCurrentQuest()) {
		QuestWidget->SelectSubGoal(this);
	}
	else {
		QuestWidget->QuestManager->SelectNewQuest(AssignedQuest, this);
	}
}

void USubGoalWidget::Update()
{

	for (int i = 0; i < AssignedQuest->CurrentGoals.Num(); i++) {
		if (GoalInfo == AssignedQuest->CurrentGoals[i]) {
			HuntIndex = i;
			break;
		}
	}

	//HuntIndex = AssignedQuest->CurrentGoals.Find(GoalInfo);

	if (GoalInfo.bCustomGoal) {
		GoalText->SetText(GoalInfo.GoalText);
	}
	else {
		FText TempText;
		FText bS;
		switch (GoalInfo.Type)
		{
		case EGoalTypes::Custom:
			TempText = GoalInfo.GoalText;
			break;
		case EGoalTypes::Hunt:
			if (GoalInfo.AmountToHunt > 1) {
				bS = FText::FromString("s");
			}
			else {
				bS = FText::FromString("");
			}
			TempText = FText::Format(LOCTEXT("SubGoalWidget", "Hunt {0}{1}:{2}/{3}"),
				GoalInfo.AdditionalName, bS,
				FText::AsNumber(AssignedQuest->CurrentHuntedAmounts[HuntIndex]),
				FText::AsNumber(GoalInfo.AmountToHunt));
			break;
		case EGoalTypes::Find:
			TempText = FText::Format(LOCTEXT("SubGoalWidget", "Find the {0}"), GoalInfo.AdditionalName);
			break;
		case EGoalTypes::Talk:
			TempText = FText::Format(LOCTEXT("SubGoalWidget", "Talk to {0}"), GoalInfo.AdditionalName);
			break;
		default:
			break;
		}
		GoalText->SetText(TempText);
	}
	for (int i = 0; i < AssignedQuest->QuestInfo.SubGoals.Num(); i++) {
		if (AssignedQuest->QuestInfo.SubGoals[i] == GoalInfo) {
			GoalIndex = i;
		}
	}
}

void USubGoalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SelectButton->OnClicked.AddDynamic(this,&USubGoalWidget::OnSelectButtonClicked);
	Update();
}

#undef LOCTEXT_NAMESPACE
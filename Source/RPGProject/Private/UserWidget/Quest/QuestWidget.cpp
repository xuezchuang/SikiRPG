// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/UserWidget/Quest/QuestWidget.h"
#include "Components/VerticalBox.h"
#include "Public/Quest/QuestStruct.h"
#include "Public/Quest/BaseQuest.h"
#include "Public/UserWidget/Quest/SubGoalWidget.h"
#include "Components/TextBlock.h"
#include "Public/Quest/QuestEnum.h"
#include "Public/Quest/QuestManager.h"
#include "Components/Border.h"
#include "Components/Button.h"

void UQuestWidget::GenerateSubWidgets()
{
	SubGoalWidgets.Empty();
	SubGoalBox->ClearChildren();
	for (FGoalInfo GoalInfo : AssignedQuest->CurrentGoals) {
		USubGoalWidget* SubGoalWidget= CreateWidget<USubGoalWidget>(GetWorld(), LoadClass<USubGoalWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Quest/WBP_SubGoal.WBP_SubGoal_C'"))) ;
		SubGoalWidget->GoalInfo = GoalInfo;
		SubGoalWidget->AssignedQuest = AssignedQuest;
		SubGoalWidget->QuestWidget = this;

		SubGoalWidgets.Add(SubGoalWidget);
		SubGoalBox->AddChild(SubGoalWidget);
	}
}

void UQuestWidget::UpdateQuest()
{
	if (AssignedQuest) {
		QuestName->SetText(AssignedQuest->QuestInfo.Name) ;

		FLinearColor TempColor;
		switch (AssignedQuest->QuestInfo.Category)
		{
		case EQuestCategories::MainQuest:
			TempColor = FLinearColor(1,0.37,0,1);//³ÈÉ«
			break;
		case EQuestCategories::SideQuest:
			TempColor = FLinearColor(0.45,0.47,1,1);//µ­À¶É«
			break;
		case EQuestCategories::Events:
			TempColor = FLinearColor(1, 0.19, 0.13,1);//µ­ºìÉ«
				break;
		default:
			break;
		}
		QuestName->SetColorAndOpacity(TempColor);
		GenerateSubWidgets();
	}
}

bool UQuestWidget::BCurrentQuest()
{
	return (AssignedQuest == QuestManager->CurrentQuest);
}

void UQuestWidget::SelectSubGoal(class USubGoalWidget* ClickedSubGoalWidget)
{
	if (SelectedSubGoalWidget) {
		SelectedSubGoalWidget->GoalBorder->SetContentColorAndOpacity(FLinearColor(1,1,1,0.5));
		SelectedSubGoalWidget->SelectButton->SetIsEnabled(true);
	}

	if (ClickedSubGoalWidget) {
		SelectedSubGoalWidget = ClickedSubGoalWidget;
		SelectedSubGoalWidget->GoalBorder->SetContentColorAndOpacity(FLinearColor::White);
		SelectedSubGoalWidget->SelectButton->SetIsEnabled(false);
		AssignedQuest->SelcetedSubgGoalIndex = SelectedSubGoalWidget->GoalIndex;
		QuestManager->OnSwitchSubQuest();
	}
	else {
		SelectedSubGoalWidget = nullptr;
	}
}

void UQuestWidget::OnQuestSelected(class USubGoalWidget* SubGoalWidget)
{
	QuestName->SetIsEnabled(true);
	SelectSubGoal(SubGoalWidget);
}

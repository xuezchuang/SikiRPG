// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestJournal.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/Quest/BaseQuest.h"
#include "Public/Quest/QuestStruct.h"
#include "Components/TextBlock.h"
#include "Public/UserWidget/Quest/GoalEntryWidget.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Public/Other/StaticLibrary.h"
#include "Components/HorizontalBox.h"
#include "Public/UserWidget/Quest/QuestListEntry.h"
#include "Public/UserWidget/Quest/QuestCategoryWidget.h"
#include "Public/Quest/QuestManager.h"
#include "Components/Button.h"
#include "Public/UserWidget/Quest/QuestWidget.h"

#define LOCTEXT_NAMESPACE "QuestJournal"

void UQuestJournal::NativeConstruct()
{
	Super::NativeConstruct();
	SelectButton->OnClicked.AddDynamic(this,&UQuestJournal::OnSelectButtonClicked);
}

void UQuestJournal::UpdateSuggestedLevelColor()
{
	FLinearColor TempColor;
	TempColor = (PlayerCharacter->GetCurrentLevel() > SelectedQuest->QuestInfo.SuggestedLevel) ? FLinearColor::Green :
		((PlayerCharacter->GetCurrentLevel() == SelectedQuest->QuestInfo.SuggestedLevel) ? FLinearColor::White : FLinearColor::Red);
	SuggestedLevel->SetColorAndOpacity(TempColor);
}

void UQuestJournal::UpdateDescription()
{
	Description->SetText(FText::FromString(SelectedQuest->CurrentDescription.ToString().Replace(TEXT("/n"), TEXT("\n"))));
}

void UQuestJournal::GenereateSubGoals()
{
	QuestGoalBox->ClearChildren();
	for (FCompletedGoal CompletedGoal : SelectedQuest->CompletedSubGoals) {
		UGoalEntryWidget* GoalEntryWidget= CreateWidget<UGoalEntryWidget>(GetWorld(), LoadClass<UGoalEntryWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Quest/WBP_GoalEntry.WBP_GoalEntry_C'")));
		GoalEntryWidget->GoalInfo = CompletedGoal.GoalInfo;
		GoalEntryWidget->Journal = this;
		GoalEntryWidget->GoalState = CompletedGoal.bSuccessful ? EGoalStates::Success : EGoalStates::Failed;
		QuestGoalBox->AddChildToVerticalBox(GoalEntryWidget);
	}
	for(int i=0;i<SelectedQuest->CurrentGoals.Num();i++)
	//for (FGoalInfo GoalInfo:SelectedQuest->CurrentGoals)
	{
		UGoalEntryWidget* GoalEntryWidget = CreateWidget<UGoalEntryWidget>(GetWorld(), LoadClass<UGoalEntryWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Quest/WBP_GoalEntry.WBP_GoalEntry_C'")));
		GoalEntryWidget->GoalInfo =SelectedQuest->CurrentGoals[i];
		GoalEntryWidget->Journal = this;
		GoalEntryWidget->GoalState = EGoalStates::Current;
		GoalEntryWidget->HuntIndex = i;
		QuestGoalBox->AddChildToVerticalBox(GoalEntryWidget);
	}
}

void UQuestJournal::UpdateDetailWindow()
{
	if (SelectedQuest) {
		QuestName->SetText(SelectedQuest->QuestInfo.Name);

		QuestCategory->SetText(FText::FromString(UStaticLibrary::GetEnumValueAsString<EQuestCategories>("EQuestCategories", SelectedQuest->QuestInfo.Category)) );

		FLinearColor TempColor;
		switch (SelectedQuest->QuestInfo.Category)
		{
		case EQuestCategories::MainQuest:
			TempColor = FLinearColor(1,0.37,0,1);
			break;
		case EQuestCategories::SideQuest:
			TempColor = FLinearColor(0.45,0.47,1,1);
			break;
		case EQuestCategories::Events:
			TempColor = FLinearColor(1,0.19,0.13,1);
			break;
		default:
			break;
		}
		QuestCategory->SetColorAndOpacity(TempColor);

		QuestRegion->SetText(FText::FromString(UStaticLibrary::GetEnumValueAsString<ERegions>("ERegions", SelectedQuest->QuestInfo.Region)));

		SuggestedLevel->SetText(FText::AsNumber(SelectedQuest->QuestInfo.SuggestedLevel));
		UpdateSuggestedLevelColor();

		if (SelectedQuest->QuestInfo.CompletionReward.Experience > 0) {
			ExpRewardBox->SetVisibility(ESlateVisibility::Visible);
			ExpReward->SetText(FText::Format(LOCTEXT("QuestJournal", "+{0} Exp"), FText::AsNumber(SelectedQuest->QuestInfo.CompletionReward.Experience)));
		}
		else {
			ExpRewardBox->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (SelectedQuest->QuestInfo.CompletionReward.Coin > 0) {
			GoldRewardBox->SetVisibility(ESlateVisibility::Visible);
			GoldReward->SetText(FText::Format(LOCTEXT("QuestJournal", "+{0} Gold"), FText::AsNumber(SelectedQuest->QuestInfo.CompletionReward.Coin)));
		}
		else {
			GoldRewardBox->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (SelectedQuest->QuestInfo.CompletionReward.Experience <= 0 && SelectedQuest->QuestInfo.CompletionReward.Coin <= 0)
		{
			RewardBox->SetVisibility(ESlateVisibility::Collapsed);
		}
			

		UpdateDescription();

		GenereateSubGoals();

		if (SelectedQuest->CurrentState == EQuestStates::CurrentQuest) {
			ButtonBox->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			ButtonBox->SetVisibility(ESlateVisibility::Collapsed);
		}

		QuestDetailsBox->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		QuestDetailsBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UQuestJournal::AddQuestEntry(class ABaseQuest* Quest)
{
	UQuestListEntry* QuestListEntry= CreateWidget<UQuestListEntry>(GetWorld(), LoadClass<UQuestListEntry>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Quest/WBP_QuestListEntry.WBP_QuestListEntry_C'")));
	QuestListEntry->Journal = this;
	QuestListEntry->AssignedQuest = Quest;
	Quest->ListEntryWidget = QuestListEntry;
	AllQuestListEntries.Add(QuestListEntry);

	switch (Quest->CurrentState)
	{
	case EQuestStates::CurrentQuest:
		CatCurrent->QuestBox->AddChild(QuestListEntry);
		break;
	case EQuestStates::CompletedQuest:
		CatCompleted->QuestBox->AddChild(QuestListEntry);
		break;
	case EQuestStates::FailedQuest:
		CatFailed->QuestBox->AddChild(QuestListEntry);
		break;
	default:
		break;
	}
}

void UQuestJournal::InitializeWithManager(class AQuestManager* QuestManagerRef)
{
	this->QuestManager = QuestManagerRef;
	PlayerCharacter = QuestManagerRef->PlayerCharacter;
}

void UQuestJournal::OnQuestClicked(class UQuestListEntry* ClickedQuestListEntry)
{
	if (ClickedQuestListEntry)
	{
		if (CurrentQuestListEntry) {
			CurrentQuestListEntry->SetIsEnabled(true);
		}
		CurrentQuestListEntry = ClickedQuestListEntry;
		SelectedQuest = CurrentQuestListEntry->AssignedQuest;
		UpdateDetailWindow();
		CurrentQuestListEntry->SetIsEnabled(false);
	}
	else {
		UpdateDetailWindow();
	}
}

void UQuestJournal::OnSelectButtonClicked()
{
	if (QuestManager->CurrentQuest != SelectedQuest) {
		QuestManager->SelectNewQuest(SelectedQuest, SelectedQuest->QuestWidget->SubGoalWidgets[0]);
	}
}

#undef LOCTEXT_NAMESPACE
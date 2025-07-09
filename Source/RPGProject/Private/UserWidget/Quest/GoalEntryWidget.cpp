// Fill out your copyright notice in the Description page of Project Settings.

#include "GoalEntryWidget.h"
#include "Components/TextBlock.h"
#include "Public/Quest/BaseQuest.h"
#include "Public/UserWidget/Quest/QuestJournal.h"
#include "Components/Image.h"

#define LOCTEXT_NAMESPACE "GoalEntryWidget"

void UGoalEntryWidget::Update()
{
	if (GoalInfo.bCustomGoal) {
		SetGoalText(GoalInfo.GoalText);
	}
	else {
		FText TempText;
		FText bS;
		int TempHuntedAmount = 0;
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

			if (GoalState == EGoalStates::Failed) {
				TempHuntedAmount = 0;
			}
			else if (GoalState == EGoalStates::Success) {
				TempHuntedAmount = GoalInfo.AmountToHunt;
			}
			else {
				TempHuntedAmount = Journal->SelectedQuest->CurrentHuntedAmounts[HuntIndex];
			}

			TempText = FText::Format(LOCTEXT("GoalEntryWidget", "Hunt {0}{1}:{2}/{3}"),
				GoalInfo.AdditionalName, bS,
				FText::AsNumber(TempHuntedAmount),
				FText::AsNumber(GoalInfo.AmountToHunt));
			break;
		case EGoalTypes::Talk:
			TempText = FText::Format(LOCTEXT("GoalEntryWidget","Talk to {0}"),GoalInfo.AdditionalName);
			break;
		case EGoalTypes::Find:
			TempText= FText::Format(LOCTEXT("GoalEntryWidget", "Find the {0}"), GoalInfo.AdditionalName);
		default:
			break;
		}
		SetGoalText(TempText);
	}
	UTexture2D* TempTexture=nullptr;
	switch (GoalState)
	{
	case EGoalStates::Current:
		TempTexture = CurrentTexture;
		break;
	case EGoalStates::Success:
		TempTexture = SuccessTexture;
		break;
	case EGoalStates::Failed:
		TempTexture = FailedTexture;
		break;
	default:
		break;
	}
	if (TempTexture) {
		GoalStateImage->SetBrushFromTexture(TempTexture);
	}
}

void UGoalEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Update();
}

void UGoalEntryWidget::SetGoalText(FText Text)
{
	GoalText->SetText(Text);
}

#undef LOCTEXT_NAMESPACE
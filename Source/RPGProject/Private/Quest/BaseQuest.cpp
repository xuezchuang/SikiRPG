// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Quest/BaseQuest.h"
#include "Public/UserWidget/Quest/QuestWidget.h"
#include "Public/UserWidget/Quest/SubGoalWidget.h"
#include "Components/VerticalBox.h"
#include "Public/Quest/QuestManager.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Public/UserWidget/Quest/QuestJournal.h"
#include "Public/UserWidget/Quest/QuestListEntry.h"


// Sets default values
ABaseQuest::ABaseQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseQuest::BeginPlay()
{
	Super::BeginPlay();
	CaculateGoalAmount();
}

void ABaseQuest::UpdateSubGoals()
{
	CurrentGoals.Empty();
	for (int i = 0; i < CurrentGoalIndices.Num(); i++) {
		CurrentGoals.Add(QuestInfo.SubGoals[CurrentGoalIndices[i]]) ;
	}
}

void ABaseQuest::SetupStartingGoals()
{
	CurrentGoalIndices.Empty();
	CurrentGoalIndices = StartingSubGoalIndices;
	UpdateSubGoals();
	CurrentDescription = QuestInfo.Description;
	CurrentHuntedAmounts.SetNum(CurrentGoalIndices.Num());
}

bool ABaseQuest::GoToNextSubGoal()
{
	if ((CurrentGoalIndices.Max() + 1) <= QuestInfo.SubGoals.Num()) {
		int NextIndex = CurrentGoalIndices.Max() + 1;
		CurrentGoalIndices.Empty();
		CurrentGoalIndices.Add(NextIndex);
		UpdateSubGoals();
		return true;
	}
	else {
		return false;
	}
}

bool ABaseQuest::BSelectedInJournal()
{
	return(QuestManager->MainUserWidget->QuestJournal->SelectedQuest&&QuestManager->MainUserWidget->QuestJournal->SelectedQuest == this);
}

// Called every frame
void ABaseQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ABaseQuest::OnSubGoalCompleted(int SubGoalIndex, bool bSuccess)
{
	if (CurrentGoalIndices.Contains(SubGoalIndex)) {

		CurrentGoalCount++;

		FGoalInfo CompletedGoal = QuestInfo.SubGoals[SubGoalIndex];

		if (CompletedGoal.BUpdateQuestDescription&&bSuccess) {
			CurrentDescription=FText::FromString(CurrentDescription.ToString() + CompletedGoal.UpdateDescription.ToString());
			if (BSelectedInJournal()) {
				QuestManager->MainUserWidget->QuestJournal->UpdateDescription();
			}
		}

		CurrentGoals.Remove(CompletedGoal);

		int WidgetIndex= CurrentGoalIndices.Find(SubGoalIndex);
		QuestWidget->SubGoalWidgets[WidgetIndex]->RemoveFromParent();

		CurrentHuntedAmounts.RemoveAt(WidgetIndex);

		QuestWidget->SubGoalWidgets.RemoveAt(WidgetIndex);
		CurrentGoalIndices.Remove(SubGoalIndex);

		if (bSuccess) {
			//子任务成功完成
			CompletedSubGoals.Add(FCompletedGoal{ SubGoalIndex, CompletedGoal, true });

			if (CompletedGoal.bFailMeansQuestFail) {
				CurrentMustSuccedCount++;
			}

			for (int i : CompletedGoal.FollowingSubGoalIndices)
			{
				CurrentGoalIndices.Add(i);
				CurrentGoals.Add(QuestInfo.SubGoals[i]);

				if (i > CurrentHuntedAmounts.Num()) {
					CurrentHuntedAmounts.Add(0);
				}

				USubGoalWidget* SubGoalWidget = CreateWidget<USubGoalWidget>(GetWorld(), LoadClass<USubGoalWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Quest/WBP_SubGoal.WBP_SubGoal_C'")));
				SubGoalWidget->GoalInfo = QuestInfo.SubGoals[i];
				SubGoalWidget->AssignedQuest = this;
				SubGoalWidget->QuestWidget = QuestWidget;

				QuestWidget->SubGoalWidgets.Add(SubGoalWidget);
				QuestWidget->SubGoalBox->AddChild(SubGoalWidget);
			}
		}
		else {
			//子任务失败
			CompletedSubGoals.Add(FCompletedGoal{ SubGoalIndex, CompletedGoal, false });
			if (CompletedGoal.bFailMeansQuestFail) {
				//子任务失败意味总任务失败
				CurrentState = EQuestStates::FailedQuest;
				QuestManager->bCurrentQuestFinished = true;
				
				for (int i:CurrentGoalIndices)
				{
					CompletedSubGoals.Add(FCompletedGoal{i,GoalAtIndex(i),false});
				}

				CurrentGoalIndices.Empty();
				CurrentHuntedAmounts.Empty();
				CurrentGoals.Empty();

				ListEntryWidget->RemoveFromParent();
				QuestWidget->RemoveFromParent();

				QuestManager->OnQuestEnd(this);
				return true;
			}
			else {
				
				for (int i : CompletedGoal.FollowingSubGoalIndices)
				{
					CurrentGoalIndices.Add(i);
					CurrentGoals.Add(QuestInfo.SubGoals[i]);

					if (i > CurrentHuntedAmounts.Num()) {
						CurrentHuntedAmounts.Add(0);
					}

					USubGoalWidget* SubGoalWidget = CreateWidget<USubGoalWidget>(GetWorld(), LoadClass<USubGoalWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Quest/WBP_SubGoal.WBP_SubGoal_C'")));
					SubGoalWidget->GoalInfo = QuestInfo.SubGoals[i];
					SubGoalWidget->AssignedQuest = this;
					SubGoalWidget->QuestWidget = QuestWidget;

					QuestWidget->SubGoalWidgets.Add(SubGoalWidget);
					QuestWidget->SubGoalBox->AddChild(SubGoalWidget);
				}

				if (CompletedGoal.BUpdateQuestDescription) {
					CurrentDescription = FText::FromString(CurrentDescription.ToString() + CompletedGoal.UpdateDescription.ToString());
					if (BSelectedInJournal()) {
						QuestManager->MainUserWidget->QuestJournal->UpdateDescription();
					}
				}
			}

		}

		//总任务是否已经完成
		if (CurrentGoalCount >= TotalGoalAmount) {
			QuestManager->bCurrentQuestFinished = true;
			//所有子任务都已经过了一遍了
			if (CurrentMustSuccedCount >= TotalMustSuccedAmount) {
				//所有必须要完成的任务都完成了
				CurrentState = EQuestStates::CompletedQuest;
			}
			else {
				CurrentState = EQuestStates::FailedQuest;
			}
			CurrentGoalIndices.Empty();
			CurrentHuntedAmounts.Empty();
			CurrentGoals.Empty();

			ListEntryWidget->RemoveFromParent();
			QuestWidget->RemoveFromParent();

			QuestManager->OnQuestEnd(this);
			return true;
		}

		if (BSelectedInJournal()) {
			QuestManager->MainUserWidget->QuestJournal->GenereateSubGoals();
		}


		if (SubGoalIndex == SelcetedSubgGoalIndex&&QuestWidget->SubGoalWidgets.Num()>0) {
			QuestWidget->SelectSubGoal(QuestWidget->SubGoalWidgets[0]);
		}

		return true;
	}
	else {
		return false;
	}
}

FGoalInfo ABaseQuest::GoalAtIndex(int Index)
{
	return QuestInfo.SubGoals[Index];
}

void ABaseQuest::CaculateGoalAmount()
{
	for (FGoalInfo GoalInfo : QuestInfo.SubGoals) {
		TotalGoalAmount++;
		if (GoalInfo.bFailMeansQuestFail) {
			TotalMustSuccedAmount++;
		}
	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestManager.h"
#include "Public/Quest/BaseQuest.h"
#include "Public/UserWidget/Quest/QuestWidget.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Components/TextBlock.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/Quest/GoalActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/UserWidget/MinimapWidget.h"
#include "Public/UserWidget/Quest/QuestJournal.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Public/UserWidget/Quest/SubGoalWidget.h"
#include "Public/UserWidget/MinimapWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

// Sets default values
AQuestManager::AQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

float AQuestManager::GetDistanceToGoal()
{
	if(PlayerCharacter!=nullptr&&CurrentGoal!=nullptr)
	return FMath::FloorToInt(FVector::Dist2D(PlayerCharacter->GetActorLocation(), CurrentGoal->GetActorLocation())/100) ;
	else
	{
		return 0;
	}
}

void AQuestManager::UpdateDirectionArrow()
{
	FRotator Rotator= UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(), CurrentGoal->GetActorLocation());
	MainUserWidget->Minimap->RotateDirectionArrow(Rotator.Yaw);
}

void AQuestManager::OnSwitchSubQuest()
{
	if (CurrentGoal) {
		CurrentGoal->Destroy();
	}
	CurrentSubGoal = CurrentQuest->QuestInfo.SubGoals[CurrentQuest->SelcetedSubgGoalIndex];

	if (CurrentSubGoal.GoalLocation.bHasLocation) {
		CurrentGoal=GetWorld()->SpawnActor<AGoalActor>(GoalActorClass, CurrentSubGoal.GoalLocation.Location, FRotator::ZeroRotator);
		CurrentGoal->InitializeMinimapIcon(CurrentSubGoal.bUseRadius, CurrentSubGoal.Radius, CurrentSubGoal.CircleColor);
		MainUserWidget->Minimap->SetDistanceText(GetDistanceToGoal());
		UpdateDirectionArrow();
		if (GetDistanceToGoal() > ShowHintDistance) {
			MainUserWidget->Minimap->SetGoalHintVisible(true);
		}
		else {
			MainUserWidget->Minimap->SetGoalHintVisible(false);
		}
	}
	else {
		CurrentGoal = nullptr;
		MainUserWidget->Minimap->SetGoalHintVisible(false);
	}
}

void AQuestManager::OnPlayerMove()
{
	if (CurrentGoal) {
		MainUserWidget->Minimap->SetDistanceText(GetDistanceToGoal());
		UpdateDirectionArrow();
		if (GetDistanceToGoal() > ShowHintDistance) {
			MainUserWidget->Minimap->SetGoalHintVisible(true);
		}
		else {
			MainUserWidget->Minimap->SetGoalHintVisible(false);
		}
	}
}

// Called when the game starts or when spawned
void AQuestManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AQuestManager::SelectNewQuest(class ABaseQuest* NewQuest, class USubGoalWidget* SubGoalWidget)
{
	if (CurrentQuest) {
		CurrentQuest->QuestWidget->QuestName->SetIsEnabled(false);
		CurrentQuest->QuestWidget->SelectSubGoal(nullptr);
	}

	CurrentQuest = NewQuest;
	CurrentQuest->QuestWidget->OnQuestSelected(SubGoalWidget);
}

bool AQuestManager::AddNewQuest(TSubclassOf<class ABaseQuest> QuestClass)
{
	if (QuestClass && !AllQuestClasses.Contains(QuestClass)) {

		AllQuestClasses.Add(QuestClass);
		ABaseQuest* TempQuest= GetWorld()->SpawnActor<ABaseQuest>(QuestClass, FVector::ZeroVector, FRotator::ZeroRotator);
		CurrentQuestActors.Add(TempQuest);
		TempQuest->SetupStartingGoals();
		TempQuest->QuestManager = this;
		TempQuest->QuestWidget= MainUserWidget->AddQuestToList(TempQuest);
		TempQuest->QuestWidget->QuestManager = this;
		TempQuest->QuestWidget->UpdateQuest();

		MainUserWidget->QuestJournal->AddQuestEntry(TempQuest);

		if (CurrentQuestActors.Num() <= 1) {
			SelectNewQuest(TempQuest, TempQuest->QuestWidget->SubGoalWidgets[0]);
		}
		PlayerCharacter->PlayAddNewQuestAnim();

		return true;
	}
	else {
		return false;
	}
}

void AQuestManager::OnEnemyKilled(TSubclassOf<class ANormalEnemy> Enemy)
{
	//for (ABaseQuest* CurrentQuest: CurrentQuestActors)
	for(int j=0;j<CurrentQuestActors.Num();j++)
	{
		for (int i = 0; i < CurrentQuestActors[j]->CurrentGoals.Num(); i++) {
			if (CurrentQuestActors[j]->CurrentGoals[i].Type == EGoalTypes::Hunt&&CurrentQuestActors[j]->CurrentGoals[i].GoalClass == Enemy) {
				CurrentQuestActors[j]->CurrentHuntedAmounts[i]++;
				if (CurrentQuestActors[j]->CurrentHuntedAmounts[i] >= CurrentQuestActors[j]->CurrentGoals[i].AmountToHunt) {
					CurrentQuestActors[j]->OnSubGoalCompleted(CurrentQuestActors[j]->CurrentGoalIndices[i],true);
				}

				if (bCurrentQuestFinished) {
					bCurrentQuestFinished = false;
					break;
				}

				CurrentQuestActors[j]->QuestWidget->SubGoalWidgets[i]->Update();

				if (CurrentQuestActors[j]->BSelectedInJournal()) {
					MainUserWidget->QuestJournal->GenereateSubGoals();
				}
			}
		}
	}
}

void AQuestManager::OnObjectFound(TSubclassOf<class ABaseQuestProp> Prop)
{
//	for (ABaseQuest* CurrentQuest: CurrentQuestActors)
		for(int j=0;j<CurrentQuestActors.Num();j++)
	{
		for (int i = 0; i < CurrentQuestActors[j]->CurrentGoals.Num(); i++) {
			if (CurrentQuestActors[j]->CurrentGoals[i].Type == EGoalTypes::Find&&CurrentQuestActors[j]->CurrentGoals[i].GoalClass == Prop) {
				CurrentQuestActors[j]->OnSubGoalCompleted(CurrentQuestActors[j]->CurrentGoalIndices[i],true);
				if (bCurrentQuestFinished) {
					bCurrentQuestFinished = false;
					break;

				}
			}
		}
	}
}

void AQuestManager::OnTalkToNPC(TSubclassOf<class ABaseNPC> NPC, int NPCId)
{
//	for (ABaseQuest* CurrentQuest : CurrentQuestActors) 
		for(int j=0;j<CurrentQuestActors.Num();j++)
	{
		for (int i = 0; i < CurrentQuestActors[j]->CurrentGoals.Num(); i++) {
			if (CurrentQuestActors[j]->CurrentGoals[i].Type == EGoalTypes::Talk&&CurrentQuestActors[j]->CurrentGoals[i].GoalClass == NPC && CurrentQuestActors[j]->CurrentGoals[i].GoalId == NPCId) {
				CurrentQuestActors[j]->OnSubGoalCompleted(CurrentQuestActors[j]->CurrentGoalIndices[i],true);
				if (bCurrentQuestFinished) {
					bCurrentQuestFinished = false;
					break;
				}
			}			
		}
	}
}

void AQuestManager::OnQuestEnd(class ABaseQuest* Quest)
{
	CurrentQuestActors.Remove(Quest);

	switch (Quest->CurrentState)
	{
	case EQuestStates::CompletedQuest:
		CompletedQuestActors.Add(Quest);
		break;
	case EQuestStates::FailedQuest:
		FailedQuestActors.Add(Quest);
		break;
	default:
		break;
	}
	MainUserWidget->QuestJournal->AddQuestEntry(Quest);

	if (CurrentQuest == Quest) {
		CurrentQuest = nullptr;

		if (CurrentGoal) {
			CurrentGoal->Destroy();
		}

		MainUserWidget->Minimap->DistanceBox->SetVisibility(ESlateVisibility::Hidden);
		MainUserWidget->Minimap->DirectionArrow->SetVisibility(ESlateVisibility::Hidden);

		if(CurrentQuestActors.Num()>0)
		if (CurrentQuestActors[0]) {
			SelectNewQuest(CurrentQuestActors[0], CurrentQuestActors[0]->QuestWidget->SubGoalWidgets[0]);
		}
	}

	if (Quest->BSelectedInJournal()) {
		MainUserWidget->QuestJournal->SelectedQuest = nullptr;
		MainUserWidget->QuestJournal->OnQuestClicked(nullptr);
	}

	if (Quest->CurrentState == EQuestStates::CompletedQuest) {
		PlayerCharacter->IncreaseCurrentExp(Quest->QuestInfo.CompletionReward.Experience);
	}
}

// Called every frame
void AQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


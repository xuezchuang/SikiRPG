// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/Quest/QuestStruct.h"
#include "BaseQuest.generated.h"

UCLASS()
class RPGPROJECT_API ABaseQuest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseQuest();

	UPROPERTY(EditAnywhere,Category=QuestInfomation)
	FQuestInfo QuestInfo;
	UPROPERTY(EditAnywhere, Category = QuestInfomation)
	TArray<int> StartingSubGoalIndices = {0};//初始的子任务的索引，初始的时候，可能有一个，也可能有多个

	UPROPERTY(VisibleAnywhere,Category=QuesInformation)
	TArray<FCompletedGoal> CompletedSubGoals;
	
	TArray<int> CurrentGoalIndices;//当前子任务的索引

	TArray<FGoalInfo> CurrentGoals;//当前的所有的任务

	int SelcetedSubgGoalIndex;//当前选择的要完成的任务的索引

	TArray<int> CurrentHuntedAmounts;//当前杀死的怪物的数量

	EQuestStates CurrentState;

	FText CurrentDescription;

	class UQuestListEntry* ListEntryWidget;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateSubGoals();

	void SetupStartingGoals();

	bool GoToNextSubGoal();

	bool BSelectedInJournal();

public:	

	int TotalGoalAmount;
	int CurrentGoalCount;
	int TotalMustSuccedAmount;
	int CurrentMustSuccedCount;

	class AQuestManager* QuestManager;
	class UQuestWidget* QuestWidget;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool OnSubGoalCompleted(int SubGoalIndex,bool bSuccess);

	FGoalInfo GoalAtIndex(int Index);

	void CaculateGoalAmount();
};

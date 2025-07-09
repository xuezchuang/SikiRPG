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
	TArray<int> StartingSubGoalIndices = {0};//��ʼ�����������������ʼ��ʱ�򣬿�����һ����Ҳ�����ж��

	UPROPERTY(VisibleAnywhere,Category=QuesInformation)
	TArray<FCompletedGoal> CompletedSubGoals;
	
	TArray<int> CurrentGoalIndices;//��ǰ�����������

	TArray<FGoalInfo> CurrentGoals;//��ǰ�����е�����

	int SelcetedSubgGoalIndex;//��ǰѡ���Ҫ��ɵ����������

	TArray<int> CurrentHuntedAmounts;//��ǰɱ���Ĺ��������

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

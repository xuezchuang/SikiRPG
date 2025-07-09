// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Public/Quest/QuestEnum.h"
#include "QuestStruct.generated.h"

USTRUCT()
struct FQuestReward//任务完成之后的奖励
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category=QuestInfo)
	int Experience;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	int Coin;
};

USTRUCT()
struct FGoalLocation//目标的位置
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, Category = QuestInfo)
	bool bHasLocation;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	FVector Location;
};

USTRUCT()
struct FGoalInfo
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, Category = QuestInfo)
	EGoalTypes Type = EGoalTypes::Custom;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	bool bCustomGoal;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	FText GoalText;//任务的记录描述
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	FText AdditionalName;//任务作用的目标
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	int AmountToHunt;//要杀死的怪物的数量
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	FGoalLocation GoalLocation;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	bool BUpdateQuestDescription;//当前任务完成，会有下一个任务的更新提示
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	FText UpdateDescription;//任务更新的描述
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	TArray<int> FollowingSubGoalIndices;//当前子任务完成之后接下来的子任务索引
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	bool bUseRadius;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	float Radius;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	FLinearColor CircleColor;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	TSubclassOf<AActor> GoalClass;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	int GoalId;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	bool bFailMeansQuestFail;

	bool operator==(const FGoalInfo& GoalInfo) {
		return(GoalInfo.Type == Type && GoalInfo.bCustomGoal == bCustomGoal && GoalInfo.GoalText.EqualTo(GoalText)
			&& GoalInfo.AdditionalName.EqualTo(AdditionalName) && GoalInfo.AmountToHunt == AmountToHunt
			&& GoalInfo.GoalLocation.bHasLocation == GoalLocation.bHasLocation&&GoalInfo.GoalLocation.Location == GoalLocation.Location
			&&GoalInfo.BUpdateQuestDescription == BUpdateQuestDescription && GoalInfo.UpdateDescription.EqualTo(UpdateDescription)
			&& GoalInfo.FollowingSubGoalIndices == FollowingSubGoalIndices&&GoalInfo.bUseRadius==bUseRadius
			&&GoalInfo.Radius==Radius&&GoalInfo.CircleColor==CircleColor&&GoalInfo.GoalClass==GoalClass
			&&GoalInfo.GoalId==GoalId&&GoalInfo.bFailMeansQuestFail==bFailMeansQuestFail);
	}
};

USTRUCT()
struct FQuestInfo
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, Category = QuestInfo)
		FText Name;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	EQuestCategories Category;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	FText Description;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	ERegions Region;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	FQuestReward CompletionReward;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	int SuggestedLevel;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	float Difficulty;
	UPROPERTY(EditAnywhere, Category = QuestInfo)
	TArray<FGoalInfo> SubGoals;
};

USTRUCT()
struct FCompletedGoal
{
	GENERATED_BODY()

	int GoalIndex;
	FGoalInfo GoalInfo;
	bool bSuccessful;
};

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuestStruct : public UObject
{
	GENERATED_BODY()
	
	
	
	
};

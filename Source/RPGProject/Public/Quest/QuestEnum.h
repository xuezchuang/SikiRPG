// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestEnum.generated.h"

UENUM()
enum class EQuestCategories {
	//任务的类型
	MainQuest,//主线任务
	SideQuest,//支线任务
	Events//事件
};

UENUM()
enum  class EGoalTypes//目标的类型
{
	Custom,//该NPC没有任务
	Hunt,//刷怪
	Find,//搜索
	Talk//对话
};

UENUM()
enum class ERegions//任务的区域
{
	Forest UMETA(DisplayName="Forest"),
	Grass UMETA(DisplayName="Grass")
};

UENUM()
enum class EQuestStates
{
	CurrentQuest,
	CompletedQuest,
	FailedQuest
};

enum class EGoalStates
{
	Current,
	Success,
	Failed
};


/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuestEnum : public UObject
{
	GENERATED_BODY()
	
	
	
	
};

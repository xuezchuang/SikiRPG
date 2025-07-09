// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestEnum.generated.h"

UENUM()
enum class EQuestCategories {
	//���������
	MainQuest,//��������
	SideQuest,//֧������
	Events//�¼�
};

UENUM()
enum  class EGoalTypes//Ŀ�������
{
	Custom,//��NPCû������
	Hunt,//ˢ��
	Find,//����
	Talk//�Ի�
};

UENUM()
enum class ERegions//���������
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

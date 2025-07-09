// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WidgetTransform.h"
#include "SkillTreeStruct.generated.h"


USTRUCT()
struct FSubTreeConnection
{
	//���ܼ������
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,Category=Skill)
	FVector2D Position;
	UPROPERTY(EditAnywhere, Category = Skill)
	FVector2D Size;
	UPROPERTY(EditAnywhere, Category = Skill)
	FWidgetTransform Transform;
	UPROPERTY(EditAnywhere, Category = Skill)
	TSubclassOf<class ABaseSkill> ForSpell;//�����������ӵ�ǰ�ü���
};

USTRUCT()
struct FSubTreeEntry {
	//��Ӧ����SkillTreeEntryWidget
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, Category = Skill)
	FVector2D Position;
	UPROPERTY(EditAnywhere, Category = Skill)
	TSubclassOf<class ABaseSkill> SpellClass;//��Entry����Ӧ�ļ���
};

USTRUCT()
struct FSubTreeContent
{
	//��ӦUSubTreeWidget,�������м��ܺ�����
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, Category = Skill)
		TArray<FSubTreeEntry> Skills;
	UPROPERTY(EditAnywhere, Category = Skill)
	TArray<FSubTreeConnection> Connections;
};

USTRUCT()
struct FTreeCategory
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, Category = Skill)
	FText Name;
	UPROPERTY(EditAnywhere, Category = Skill)
	FSubTreeContent Content;//�ð�ť����Ӧ��SubTreeContent
};

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USkillTreeStruct : public UObject
{
	GENERATED_BODY()
	
	
	
	
};

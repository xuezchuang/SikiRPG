// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SkillDragOperation.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USkillDragOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	class USkillHotkey* FromHotkey;//��ǰ���������һ����ݼ������Ǹ���ݼ���ק������
	class ABaseSkill* SkillActor;//��ǰ��ק������Ӧ������һ������
	
};

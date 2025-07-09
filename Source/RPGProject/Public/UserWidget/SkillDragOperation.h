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
	class USkillHotkey* FromHotkey;//当前保存的是哪一个快捷键，从那个快捷键拖拽出来的
	class ABaseSkill* SkillActor;//当前拖拽的所对应的是哪一个技能
	
};

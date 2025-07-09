// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConnectionWidget.generated.h"

/**
 * 用来连接两个技能的连线
 */
UCLASS()
class RPGPROJECT_API UConnectionWidget : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(Meta=(BindWidget))
		class UImage* Line;//连线，当没有学习前置技能的时候，Alpha为0.2，学习之后Alpha为1

public:

	bool bActive;

	TSubclassOf<class ABaseSkill> ForSpell;
	void Activate();
};

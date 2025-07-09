// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConnectionWidget.generated.h"

/**
 * ���������������ܵ�����
 */
UCLASS()
class RPGPROJECT_API UConnectionWidget : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(Meta=(BindWidget))
		class UImage* Line;//���ߣ���û��ѧϰǰ�ü��ܵ�ʱ��AlphaΪ0.2��ѧϰ֮��AlphaΪ1

public:

	bool bActive;

	TSubclassOf<class ABaseSkill> ForSpell;
	void Activate();
};

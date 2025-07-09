// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/Quest/QuestStruct.h"
#include "SubGoalWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USubGoalWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* GoalText;

	UFUNCTION()
	void OnSelectButtonClicked();

public:

	UPROPERTY(Meta = (BindWidget))
		class UButton* SelectButton;

	UPROPERTY(Meta = (BindWidget))
		class UBorder* GoalBorder;
	FGoalInfo GoalInfo;
	class ABaseQuest* AssignedQuest;
	class UQuestWidget* QuestWidget;
	int GoalIndex;

	int HuntIndex;

	void Update();
	
	virtual void NativeConstruct() override;
};

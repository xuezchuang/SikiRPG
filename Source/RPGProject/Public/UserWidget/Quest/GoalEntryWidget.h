// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/Quest/QuestEnum.h"
#include "Public/Quest/QuestStruct.h"
#include "GoalEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UGoalEntryWidget : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(Meta=(BindWidget))
	class UImage* GoalStateImage;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* GoalText;

	UPROPERTY(EditAnywhere,Category="StateIcon")
	UTexture2D* CurrentTexture;
	UPROPERTY(EditAnywhere, Category = "StateIcon")
	UTexture2D* SuccessTexture;
	UPROPERTY(EditAnywhere, Category = "StateIcon")
	UTexture2D* FailedTexture;

	void Update();

	virtual void NativeConstruct() override;
	
public:

	FGoalInfo GoalInfo;

	EGoalStates GoalState;

	class UQuestJournal* Journal;
	void SetGoalText(FText Text);

	int HuntIndex;
};

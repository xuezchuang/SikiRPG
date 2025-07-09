// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/Quest/QuestEnum.h"
#include "QuestCategoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuestCategoryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta=(BindWidget))
	class UButton* ExpandButton;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* QuestCategoryName;


	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere)
	EQuestStates CurrentState = EQuestStates::CurrentQuest;

	bool bExpand = false;

	UFUNCTION()
	void OnExpandButtonClicked();

public:
	UPROPERTY(Meta = (BindWidget))
	class UVerticalBox* QuestBox;
	void SetCategoryName(FText Text);
};

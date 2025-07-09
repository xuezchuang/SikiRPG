// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestListEntry.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuestListEntry : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta=(BindWidget))
	UTextBlock* QuestName;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* RegionName;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* SuggestedLevel;
	UPROPERTY(Meta = (BindWidget))
	class UButton* QuestButton;


	FString GetRegionEnumString(ERegions Region);

	virtual void NativeConstruct() override;

public:
	class UQuestJournal* Journal;

	class ABaseQuest* AssignedQuest;
	void SetQuestName(FText QuestNameText);
	void SetRegionName(FText RegionNameText);
	void SetSuggesteLevel(FText LevelText);

	UFUNCTION()
		void OnQuestButtonClicked();

	void UpdateLevelColor();

	void Update();
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestCategoryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"

void UQuestCategoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	switch (CurrentState)
	{
	case EQuestStates::CurrentQuest:
		SetCategoryName(FText::FromString("Current Quest"));
		break;
	case EQuestStates::CompletedQuest:
		SetCategoryName(FText::FromString("Completed Quest"));
		break;
	case EQuestStates::FailedQuest:
		SetCategoryName(FText::FromString("Failed Quest"));
		break;
	default:
		break;
	}
	ExpandButton->OnClicked.AddDynamic(this,&UQuestCategoryWidget::OnExpandButtonClicked);
}

void UQuestCategoryWidget::OnExpandButtonClicked()
{
	bExpand = !bExpand;
	//ExpandButton->SetRenderAngle(bExpand ? 0 : -90);
	QuestBox->SetVisibility(bExpand ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UQuestCategoryWidget::SetCategoryName(FText Text)
{
	QuestCategoryName->SetText(Text);
}

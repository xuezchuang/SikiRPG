// Fill out your copyright notice in the Description page of Project Settings.

#include "MinimapWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

void UMinimapWidget::RotateDirectionArrow(float Angle)
{
	//DirectionArrow->SetRenderAngle(Angle);
}

void UMinimapWidget::SetDistanceText(float Distance)
{
	DistanceText->SetText(FText::AsNumber(Distance));
}

void UMinimapWidget::SetGoalHintVisible(bool bVisible)
{
	if (bVisible)
	{
		DirectionArrow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		DistanceBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		DirectionArrow->SetVisibility(ESlateVisibility::Hidden);
		DistanceBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

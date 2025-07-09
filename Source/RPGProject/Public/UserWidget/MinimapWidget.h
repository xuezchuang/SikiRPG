// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

/**
 *
 */
UCLASS()
class RPGPROJECT_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()


	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* DistanceText;


public:
	UPROPERTY(Meta = (BindWidget))
	class UImage* DirectionArrow;
	UPROPERTY(Meta = (BindWidget))
	class UHorizontalBox* DistanceBox;
	void RotateDirectionArrow(float Angle);
	void SetDistanceText(float Distance);
	void SetGoalHintVisible(bool bVisible);
};

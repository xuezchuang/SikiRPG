// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionWidget.h"
#include "Components/TextBlock.h"



void UInteractionWidget::SetNameText(FText Name)
{
	NameText->SetText(Name);
}

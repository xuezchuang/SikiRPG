// Fill out your copyright notice in the Description page of Project Settings.

#include "MessageWidget.h"
#include "Components/TextBlock.h"

void UMessageWidget::SetMessage(FText Text)
{
	Message->SetText(Text);
}

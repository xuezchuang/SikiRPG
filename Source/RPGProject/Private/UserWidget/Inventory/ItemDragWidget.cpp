// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemDragWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UItemDragWidget::Init(int Amount, UTexture2D* Icon)
{
	AmountText->SetText(FText::AsNumber(Amount));
	IconImage->SetBrushFromTexture(Icon);
}

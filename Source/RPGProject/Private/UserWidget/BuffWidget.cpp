// Fill out your copyright notice in the Description page of Project Settings.

#include "BuffWidget.h"
#include "Components/Image.h"



void UBuffWidget::NativeConstruct()
{
	BuffIcon->SetBrushFromTexture(BuffTexture);
	CooldownMat = CooldownImage->GetDynamicMaterial();
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "ReadableItemWidget.h"
#include "Public/InveotorySystem/ItemMap.h"
#include "Components/Button.h"


void UReadableItemWidget::OnCloseButtonClicked()
{
	ItemMap->OnReadOver();
}

void UReadableItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this,&UReadableItemWidget::OnCloseButtonClicked);
}

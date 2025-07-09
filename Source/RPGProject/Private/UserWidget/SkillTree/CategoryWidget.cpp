// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/UserWidget/SkillTree/CategoryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Public/UserWidget/SkillTree/MainTreeWidget.h"

void UCategoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CategoryNameText->SetText(CategoryName);
	CategoryButton->OnHovered.AddDynamic(this, &UCategoryWidget::OnCategoryButtonHoverd);
	CategoryButton->OnUnhovered.AddDynamic(this, &UCategoryWidget::OnCategoryButtonUnHoverd);
	CategoryButton->OnClicked.AddDynamic(this, &UCategoryWidget::OnCategoryButtonClicked);
}

void UCategoryWidget::OnCategoryButtonHoverd()
{
	CategoryNameText->SetColorAndOpacity(FLinearColor(0.83, 0.83, 0.83, 1));
}

void UCategoryWidget::OnCategoryButtonUnHoverd()
{
	if (!bSelected)
	{
		CategoryNameText->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1));
	}
}

void UCategoryWidget::OnCategoryButtonClicked()
{
	MainTreeRef->OnCategoryClicked(Index);
}

void UCategoryWidget::ChangeState(bool bSelectedRef)
{
	if (bSelectedRef != this->bSelected)
	{
		this->bSelected = bSelectedRef;

		FLinearColor TempColor;
		if (bSelectedRef)
		{
			TempColor = FLinearColor::White;
		}
		else
		{
			TempColor = FLinearColor(0.4f, 0.4f, 0.4f, 1);
		}

		CategoryNameText->SetColorAndOpacity(TempColor);
		CategoryButton->SetIsEnabled(!bSelectedRef);
	}
}

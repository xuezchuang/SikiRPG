// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/UserWidget/SkillTree/SkillTreeEntryWidget.h"
#include "Public/Character/Skill/BaseSkill.h"
#include "Public/Character/RBaseCharacter.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SkillTreeComponent.h"
#include "Public//UserWidget/SkillTree/MainTreeWidget.h"
#include "Public//UserWidget/SkillTree/SubTreeWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "WidgetBlueprintLibrary.h"
#include "SkillDrag.h"
#include "SkillDragOperation.h"

#define LOCTEXT_NAMESPACE "SkillEntryWidget"

void USkillTreeEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SkillActor=GetWorld()->SpawnActor<ABaseSkill>(SkillClass,FVector::ZeroVector,FRotator::ZeroRotator);
	SkillActor->SetPlayerReference(SubTree->MainTree->SkillTreeComp->PlayerCharacter);
	UpdateStageText();
	UpdateIcon();
	UpdateUpgradeBox();
	PlusButton->OnClicked.AddDynamic(this,&USkillTreeEntryWidget::OnPlusButtonClicked);
	MinusButton->OnClicked.AddDynamic(this, &USkillTreeEntryWidget::OnMinusButtonClicked);
}

int USkillTreeEntryWidget::GetAmountOfStages()
{
	return SkillActor->GetSkillInfo().Stages.Num();
}

void USkillTreeEntryWidget::UpdateStageText()
{
	StageText->SetText(FText::Format(LOCTEXT("SkillEntryWidget","{0}/{1}"),SkillActor->GetCurrentStageIndex()+1,FText::AsNumber(GetAmountOfStages())));
}

void USkillTreeEntryWidget::UpdateIcon()
{
	UTexture2D* TempTexture;

	if (SkillActor->GetCurrentStageIndex() < 0) {
		TempTexture = SkillActor->GetSkillInfo().Icon;
	}
	else {
		if (SkillActor->GetCurrentStage().OverrideIcon) {
			TempTexture = SkillActor->GetCurrentStage().OverrideIcon;
		}
		else {
			TempTexture = SkillActor->GetSkillInfo().Icon;
		}
	}
	SkillIcon->SetBrushFromTexture(TempTexture);
}

void USkillTreeEntryWidget::UpdateUpgradeBox()
{
	if (SubTree->MainTree->SkillTreeComp->BCanUpgradeSpell(SkillActor)) {
		PlusButton->SetVisibility(ESlateVisibility::Visible);
		MinusButton->SetVisibility(ESlateVisibility::Hidden);
		UpgradeBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (SkillActor->GetCurrentStageIndex() >= 1) {
			MinusButton->SetVisibility(ESlateVisibility::Visible);
			
		}
		SetColorAndOpacity(FLinearColor::White);
	}
	else {
		PlusButton->SetVisibility(ESlateVisibility::Hidden);
		if (SkillActor->GetCurrentStageIndex() >= 1) {
			MinusButton->SetVisibility(ESlateVisibility::Visible);
			UpgradeBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SetColorAndOpacity(FLinearColor::White);
		}
		else {
			if (SubTree->MainTree->SkillTreeComp->BPlayerLearnedSpell(SkillClass)) {
				SetColorAndOpacity(FLinearColor::White);
				UpgradeBox->SetVisibility(ESlateVisibility::Hidden);
			}
			else {
				SetColorAndOpacity(FLinearColor(1, 1, 1, 0.3f));
				UpgradeBox->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void USkillTreeEntryWidget::OnSpellLearnt()
{
	bSpellLearnt = true;
	SkillIcon->SetColorAndOpacity(FLinearColor::White);
}

void USkillTreeEntryWidget::OnPlusButtonClicked()
{
	SubTree->MainTree->SkillTreeComp->UpgradeSpell(SkillActor, this);
}

void USkillTreeEntryWidget::OnMinusButtonClicked()
{
	SubTree->MainTree->SkillTreeComp->DowngradeSpell(SkillActor, this);
}

FReply USkillTreeEntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

		if (bSpellLearnt) {
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton).NativeReply;
		}
		else {
			return FReply::Handled();
		}

}

void USkillTreeEntryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (!SubTree->MainTree->SkillTreeComp->BSkillOnHotkey(SkillActor))
	{
		USkillDrag* SkillDrag = CreateWidget<USkillDrag>(GetWorld(), LoadClass<USkillDrag>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/WBP_SkillDrag.WBP_SkillDrag_C'")));

		if (SkillActor->GetCurrentStage().OverrideIcon) {
			SkillDrag->SetSkillTexture(SkillActor->GetCurrentStage().OverrideIcon);
		}
		else {
			SkillDrag->SetSkillTexture(SkillActor->GetSkillInfo().Icon);
		}

		UDragDropOperation* TempOp = UWidgetBlueprintLibrary::CreateDragDropOperation(SkillDragOperation);
		TempOp->DefaultDragVisual = SkillDrag;
		OutOperation = TempOp;

		if (Cast<USkillDragOperation>(OutOperation)) {
			Cast<USkillDragOperation>(OutOperation)->SkillActor = SkillActor;
			Cast<USkillDragOperation>(OutOperation)->FromHotkey = nullptr;
		}
	}
}

#undef LOCTEXT_NAMESPACE
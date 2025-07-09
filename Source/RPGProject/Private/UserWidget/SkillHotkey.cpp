// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/UserWidget/SkillHotkey.h"
#include "Components/TextBlock.h"
#include "Public/Character/Skill/BaseSkill.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Public/UserWidget/SkillDrag.h"
#include "Public/UserWidget/SkillDragOperation.h"

bool USkillHotkey::Initialize()
{
	if (!Super::Initialize()) {
		return false;
	}
	SkillButton->OnClicked.AddDynamic(this,&USkillHotkey::OnSkillButtonClicked);
	CooldownMat = CooldownImage->GetDynamicMaterial();
	DragOverColor = FLinearColor(1.0f,0.82f,0.0f,0.5f);
	DefaultColor = FLinearColor(0,0,0,0.2f);
	return true;
}

void USkillHotkey::OnSkillButtonClicked()
{
	AssignedSpell->OnTryCastSpell();
}

void USkillHotkey::ResetStyle()
{
	bDraggedOver = false;
	Base->SetColorAndOpacity(DefaultColor);
}

FReply USkillHotkey::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);

	if (AssignedSpell) {
		if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && !AssignedSpell->GetBOnCooldown()) {
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton).NativeReply;
		}
		else {
			return FReply::Handled();
		}
	}
	else {
		return FReply::Handled();
	}
}

void USkillHotkey::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	USkillDrag* SkillDrag= CreateWidget<USkillDrag>(GetWorld(), LoadClass<USkillDrag>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/WBP_SkillDrag.WBP_SkillDrag_C'")));

	if (AssignedSpell->GetCurrentStage().OverrideIcon) {
		SkillDrag->SetSkillTexture(AssignedSpell->GetCurrentStage().OverrideIcon);
	}
	else {
		SkillDrag->SetSkillTexture(AssignedSpell->GetSkillInfo().Icon);
	}

	UDragDropOperation* TempOp= UWidgetBlueprintLibrary::CreateDragDropOperation(SkillDragOperation);
	TempOp->DefaultDragVisual = SkillDrag;
	OutOperation = TempOp;

	if (Cast<USkillDragOperation>(OutOperation)) {
		Cast<USkillDragOperation>(OutOperation)->SkillActor = AssignedSpell;
		Cast<USkillDragOperation>(OutOperation)->FromHotkey = this;
	}
}

bool USkillHotkey::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//是被放上去拖拽物的时候
	Super::NativeOnDragOver(InGeometry,InDragDropEvent,InOperation);

	//InOperation是之前从别的地方拽出来的OutOperation
	if (Cast<USkillDragOperation>(InOperation)) {
		if (!bDraggedOver&&Cast<USkillDragOperation>(InOperation)->FromHotkey != this&&AssignedSpell == nullptr) {
			Base->SetColorAndOpacity(DragOverColor);
			bDraggedOver = true;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void USkillHotkey::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent,InOperation);

	if (bDraggedOver) {
		//因为OnLeave函数并不仅仅是在拖拽到另外一个插槽上又离开的时候调用的，
		//所以需要排除一下从当前插槽离开的情况
		if (Cast<USkillDragOperation>(InOperation)) {
			ResetStyle();
		}
	}
}

bool USkillHotkey::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry,InDragDropEvent,InOperation);

	if (Cast<USkillDragOperation>(InOperation)) {
		if (Cast<USkillDragOperation>(InOperation)->FromHotkey != this) {
			if (AssignedSpell) {
				if (AssignedSpell->GetBOnCooldown()) {
					return true;
				}
				else {
					ABaseSkill* LocalAssignedSpell = AssignedSpell;
					ClearAssignedSpell();
					if (Cast<USkillDragOperation>(InOperation)->FromHotkey)
					{
						Cast<USkillDragOperation>(InOperation)->FromHotkey->ClearAssignedSpell();
						AssignSpell(Cast< USkillDragOperation>(InOperation)->SkillActor);
						Cast<USkillDragOperation>(InOperation)->FromHotkey->AssignSpell(LocalAssignedSpell);
					}
					else {
						AssignSpell(Cast< USkillDragOperation>(InOperation)->SkillActor);
					}
					ResetStyle();
					return true;
				}
			}
			else {
				if(Cast<USkillDragOperation>(InOperation)->FromHotkey)
				Cast<USkillDragOperation>(InOperation)->FromHotkey->ClearAssignedSpell();
				AssignSpell(Cast<USkillDragOperation>(InOperation)->SkillActor);
				return true;
			}
		}
		else {
			return false;
		}
	}
	else
		return false;
}

void USkillHotkey::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!bHasSetKeyName)
	{
		HotkeyName->SetText(Key.GetDisplayName());
		bHasSetKeyName = true;
	}
}

void USkillHotkey::AssignSpell(ABaseSkill* NewAssigneSpell)
{
	this->AssignedSpell = NewAssigneSpell;
	AssignedSpell->SetHotkey(this);
	SkillButton->SetIsEnabled(true);
	if (AssignedSpell->GetCurrentStage().OverrideIcon) {
		SkillIcon->SetBrushFromTexture(AssignedSpell->GetCurrentStage().OverrideIcon);
	}
	else {
		SkillIcon->SetBrushFromTexture(AssignedSpell->GetSkillInfo().Icon);
	}
	SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void USkillHotkey::ClearAssignedSpell()
{
	if (AssignedSpell) {
		AssignedSpell->SetHotkey(nullptr);
		AssignedSpell = nullptr;

		SkillButton -> SetIsEnabled(false);
		SkillIcon->SetBrushFromTexture(nullptr);
		SkillIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USkillHotkey::EnableHotkey()
{
	bDeactived = false;
	if (!AssignedSpell->GetBOnCooldown()) {
		SkillButton->SetIsEnabled(true);
		SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CooldownImage->SetVisibility(ESlateVisibility::Hidden);
		SkillIcon->SetColorAndOpacity(FLinearColor::White);
	}
}

void USkillHotkey::DisableHotkey()
{
	bDeactived = true;
	if (!AssignedSpell->GetBOnCooldown()) {
		SkillButton->SetIsEnabled(false);
		SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CooldownImage->SetVisibility(ESlateVisibility::Visible);
		CooldownMat->SetScalarParameterValue(FName("Percent"), 1.0f);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Components/SkillTreeComponent.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/Character/Skill/BaseSkill.h"
#include "Public/UserWidget/SkillHotkey.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Public/UserWidget/SkillTree/MainTreeWidget.h"
#include "Public/UserWidget/SkillTree/ConnectionWidget.h"
#include "Public/UserWidget/SkillTree/SubTreeWidget.h"
#include "Public/UserWidget/SkillTree/SkillTreeEntryWidget.h"
#include "TextBlock.h"

// Sets default values for this component's properties
USkillTreeComponent::USkillTreeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ARBaseCharacter>(GetOwner());

}


// Called every frame
void USkillTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USkillTreeComponent::BPlayerLearnedSpell(TSubclassOf<ABaseSkill> Skill)
{
	return PlayerCharacter->LearntSkills.Contains(Skill);
}

void USkillTreeComponent::AddSkillPoints(int Amount)
{
	SkillPoints += Amount;
	MainTreeWidget->SpText->SetText(FText::AsNumber(SkillPoints));
	UpdateAllEntries();
}

bool USkillTreeComponent::BCanUpgradeSpell(ABaseSkill* Spell)
{
	//1.技能点大于0，2.主角当前的等级>该技能下一阶段所需要的等级，比如大招，3.技能不能够冒出去
	if (SkillPoints > 0 && PlayerCharacter->GetCurrentLevel() >= Spell->GetNextStage().RequiredLevel && Spell->GetCurrentStageIndex() < Spell->GetSkillInfo().Stages.Num() - 1)
	{
		bool LocalBool = true;
		for (TSubclassOf<ABaseSkill> PreSkill : Spell->GetNextStage().RequiredSkills)
		{
			if (!BPlayerLearnedSpell(PreSkill))
			{
				//有的技能需要很多前置技能，如果有一个没有学习，则返回假
				LocalBool = false;
				break;
			}
		}
		return LocalBool;
	}
	else
	{
		return false;
	}
}

bool USkillTreeComponent::BSkillOnHotkey(ABaseSkill* Spell)
{
	bool LocalBool = false;
	for (USkillHotkey* TempHotkey : PlayerCharacter->MainUserWidget->GetAllHotkeySlots())
	{
		if (TempHotkey->GetAssignedSpell() && TempHotkey->GetAssignedSpell() == Spell)
		{
			LocalBool = true;
			break;
		}
	}
	return LocalBool;
}

void USkillTreeComponent::UpgradeSpell(ABaseSkill* SkillActor, class USkillTreeEntryWidget* EntryWidgetRef)
{
	this->UpgradedSpell = SkillActor;
	this->EntryWidget = EntryWidgetRef;

	if (BCanUpgradeSpell(SkillActor))
	{
		SkillPoints -= 1;
		UpgradedSpell->ChangeCurrentStageIndex(1);
		if (UpgradedSpell->GetCurrentStageIndex() == 0)
		{
			PlayerCharacter->LearntSkills.Add(UpgradedSpell->GetClass());
			EntryWidget->OnSpellLearnt();
			ActiveConnections(SkillActor->GetClass());

			EntryWidget->UpdateStageText();
			EntryWidget->UpdateIcon();
			MainTreeWidget->UpdateSp();
			UpdateAllEntries();
		}
		else
		{
			EntryWidget->UpdateStageText();
			EntryWidget->UpdateIcon();
			MainTreeWidget->UpdateSp();
			UpdateAllEntries();
		}
	}
}

void USkillTreeComponent::DowngradeSpell(ABaseSkill* SkillActor, class USkillTreeEntryWidget* EntryWidgetRef)
{
	this->UpgradedSpell = SkillActor;
	this->EntryWidget = EntryWidgetRef;

	UpgradedSpell->ChangeCurrentStageIndex(-1);
	SkillPoints += 1;
	EntryWidget->UpdateStageText();
	EntryWidget->UpdateIcon();
	MainTreeWidget->UpdateSp();
	UpdateAllEntries();
}

void USkillTreeComponent::HandleShowCommand()
{
	if (bTreeSetup)
	{
		if (PlayerCharacter->MainUserWidget->bTreeShown)
		{
			MainTreeWidget->SetVisibility(ESlateVisibility::Hidden);
			PlayerCharacter->MainUserWidget->bTreeShown = false;
			FInputModeGameAndUI InputMode;
			PlayerCharacter->PC->SetInputMode(InputMode);
		}
		else
		{
			MainTreeWidget->SetVisibility(ESlateVisibility::Visible);
			FInputModeUIOnly InputMode;
			PlayerCharacter->PC->SetInputMode(InputMode);
			PlayerCharacter->MainUserWidget->bTreeShown = true;
		}
	}
}

void USkillTreeComponent::SetupTree()
{
	MainTreeWidget = PlayerCharacter->MainUserWidget->SkillTree;
	if (MainTreeWidget)
	{
		MainTreeWidget->Initialze(this);
		bTreeSetup = true;
	}
}

void USkillTreeComponent::ActiveConnections(TSubclassOf<ABaseSkill> ForSpell)
{
	for (USubTreeWidget* SubTreeWidget : MainTreeWidget->SubTreeWidgets)
	{
		for (UConnectionWidget* ConnectionWidget : SubTreeWidget->Connections)
		{
			if (ConnectionWidget->ForSpell == ForSpell)
			{
				ConnectionWidget->Activate();
			}
		}
	}
}

void USkillTreeComponent::UpdateAllEntries()
{
	for (USubTreeWidget* SubTreeWidget : MainTreeWidget->SubTreeWidgets)
	{
		for (USkillTreeEntryWidget* SkillTreeEntry : SubTreeWidget->SkillEntries)
		{
			SkillTreeEntry->UpdateUpgradeBox();
		}
	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestListEntry.h"
#include "Components/TextBlock.h"
#include "Public/UserWidget/Quest/QuestJournal.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/Quest/BaseQuest.h"
#include "Public/Quest/QuestStruct.h"
#include "Components/Button.h"

void UQuestListEntry::UpdateLevelColor()
{
	FLinearColor TempColor;
	if (Journal->PlayerCharacter->GetCurrentLevel() > AssignedQuest->QuestInfo.SuggestedLevel)
	{
		TempColor = FLinearColor::Green;
	}
	else if (Journal->PlayerCharacter->GetCurrentLevel() == AssignedQuest->QuestInfo.SuggestedLevel)
	{
		TempColor = FLinearColor::White;
	}
	else
	{
		TempColor = FLinearColor::Red;
	}
	SuggestedLevel->SetColorAndOpacity(TempColor);
}

void UQuestListEntry::Update()
{
	if (AssignedQuest->QuestInfo.Name.ToString().Len() < 20)
	{
		SetQuestName(AssignedQuest->QuestInfo.Name);
	}
	else
	{
		SetQuestName(FText::FromString(AssignedQuest->QuestInfo.Name.ToString().LeftChop(AssignedQuest->QuestInfo.Name.ToString().Len() - 17).Append("...")));
	}

	SetSuggesteLevel(FText::AsNumber(AssignedQuest->QuestInfo.SuggestedLevel));
	UpdateLevelColor();

	SetRegionName(FText::FromString(GetRegionEnumString(AssignedQuest->QuestInfo.Region)));
}

void UQuestListEntry::SetQuestName(FText QuestNameText)
{
	QuestName->SetText(QuestNameText);
}

void UQuestListEntry::SetRegionName(FText RegionNameText)
{
	RegionName->SetText(RegionNameText);
}

void UQuestListEntry::SetSuggesteLevel(FText LevelText)
{
	SuggestedLevel->SetText(LevelText);
}

void UQuestListEntry::OnQuestButtonClicked()
{
	Journal->OnQuestClicked(this);
}

FString UQuestListEntry::GetRegionEnumString(ERegions Region)
{
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ERegions"), true);
	const UEnum* EnumPtr = StaticEnum<ERegions>();
	if (!EnumPtr)
	{
		return FString("InValid");
	}
	FString TempString = EnumPtr->GetNameByValue((int64)Region).ToString();
	TempString.RemoveFromStart("ERegions::");
	return TempString;
}

void UQuestListEntry::NativeConstruct()
{
	Super::NativeConstruct();
	Update();
	QuestButton->OnClicked.AddDynamic(this, &UQuestListEntry::OnQuestButtonClicked);
}

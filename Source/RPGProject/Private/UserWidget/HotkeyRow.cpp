// Fill out your copyright notice in the Description page of Project Settings.

#include "HotkeyRow.h"
#include "Public/UserWidget/SkillHotkey.h"
#include "Components/HorizontalBox.h"

TArray<USkillHotkey*> UHotkeyRow::GenerateHotkeys()
{
	for (auto Key : Keys)
	{
		USkillHotkey* SkillHotkey = CreateWidget<USkillHotkey>(GetWorld(), LoadClass<USkillHotkey>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/WBP_SkillHotkey.WBP_SkillHotkey_C'")));
		SkillHotkey->SetKey(Key);
		SlotWidgets.Add(SkillHotkey);
		RowBox->AddChildToHorizontalBox(SkillHotkey);
	}
	return SlotWidgets;
}

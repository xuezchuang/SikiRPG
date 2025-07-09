// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HotkeyRow.generated.h"

class USkillHotkey;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UHotkeyRow : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta=(BindWidget))
	class UHorizontalBox* RowBox;//�����һ����������SkillHotkey
	
	TArray<FKey> Keys;//��һ������Ҫ�İ������Ǵ��ⲿ���ݹ����ģ�������HotkeyRow֮�󣬱���ֵ

	TArray<USkillHotkey*> SlotWidgets;//��һ��ӵ�е�SkillHotKey;

public:
	FORCEINLINE void SetHotkeys(TArray<FKey> KeysRef) { this->Keys = KeysRef; }

	TArray<USkillHotkey*> GenerateHotkeys();
};

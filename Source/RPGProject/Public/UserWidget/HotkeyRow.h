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
	class UHorizontalBox* RowBox;//存放着一排所创建的SkillHotkey
	
	TArray<FKey> Keys;//这一排所需要的按键，是从外部传递过来的，创建该HotkeyRow之后，被赋值

	TArray<USkillHotkey*> SlotWidgets;//这一排拥有的SkillHotKey;

public:
	FORCEINLINE void SetHotkeys(TArray<FKey> KeysRef) { this->Keys = KeysRef; }

	TArray<USkillHotkey*> GenerateHotkeys();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/Character/Skill/SkillTreeStruct.h"
#include "SubTreeWidget.generated.h"

/**
 * 与种类相对应的下面的界面
 */
UCLASS()
class RPGPROJECT_API USubTreeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta=(BindWidget))
	class UCanvasPanel* Canvas;
	
	FSubTreeContent Content;
	TArray<class USkillTreeEntryWidget*> SkillEntries;
	TArray<class UConnectionWidget*> Connections;
	class UMainTreeWidget* MainTree;

	virtual void NativeConstruct() override;

public:
	void GenerateContent();
	
};

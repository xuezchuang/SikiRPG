// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/Character/Skill/SkillTreeStruct.h"
#include "MainTreeWidget.generated.h"

class UTextBlock;

/**
 * 技能树主容器
 */
UCLASS()
class RPGPROJECT_API UMainTreeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* SpText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* LevelText;
	UPROPERTY(Meta = (BindWidget))
	class UScrollBox* CategoriesBox;
	UPROPERTY(Meta = (BindWidget))
	class UWidgetSwitcher* SubTreeSwitcher;
	UPROPERTY(Meta = (BindWidget))
	class UButton* CloseButton;

	class USkillTreeComponent* SkillTreeComp;
	int CurrentlySelectedIndex = -1;//当前选择的Category所对应的索引
	TArray<class UCategoryWidget*> CategoryWidgets;//分类，比如Skill,Defense...
	TArray<FTreeCategory> TreeCategories;//CategoryWidget的信息
	TArray<class USubTreeWidget*> SubTreeWidgets;//Category所对应的Content

	virtual void NativeConstruct() override;

	void OnCategoryClicked(int Index);

	UFUNCTION()
	void OnCloseButtonClicked();

	void UpdateLevel();

	void UpdateSp();

	void GenerateCategories();

	void Initialze(class USkillTreeComponent* AssignedTreeComp);

};

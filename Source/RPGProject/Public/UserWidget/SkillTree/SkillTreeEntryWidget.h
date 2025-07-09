// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeEntryWidget.generated.h"

class UImage;
class UButton;

/**
 * ���ܲ��ͼ��
 */
UCLASS()
class RPGPROJECT_API USkillTreeEntryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta=(BindWidget))
	class UOverlay* SkillOverlay;
	UPROPERTY(Meta = (BindWidget))
	UImage* SkillIcon;
	UPROPERTY(Meta = (BindWidget))
	UImage* SkillBorder;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* StageText;
	UPROPERTY(Meta = (BindWidget))
	class UVerticalBox* UpgradeBox;
	UPROPERTY(Meta = (BindWidget))
	UButton* MinusButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* PlusButton;

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USkillDragOperation> SkillDragOperation;
public:
	class USubTreeWidget* SubTree;
	class ABaseSkill* SkillActor;
	TSubclassOf<class ABaseSkill> SkillClass;
	bool bSpellLearnt;
	int GetAmountOfStages();
	void UpdateStageText();
	void UpdateIcon();
	void UpdateUpgradeBox();
	void OnSpellLearnt();

	UFUNCTION()
		void OnPlusButtonClicked();

	UFUNCTION()
		void OnMinusButtonClicked();
	//ֻ�е�����Ҽ����µ�ʱ�򣬲���ִ�м����ק�ķ���
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//����⵽��ק��ʱ����õĺ���
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};

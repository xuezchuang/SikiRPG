// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputCoreTypes.h"
#include "SkillHotkey.generated.h"

class UTextBlock;
class UImage;
class ABaseSkill;
/**
 *
 */
UCLASS()
class RPGPROJECT_API USkillHotkey : public UUserWidget
{
	GENERATED_BODY()
	bool bHasSetKeyName = false;

	virtual bool Initialize() override;

public:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* HotkeyName;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* CooldownText;

	UPROPERTY(Meta = (BindWidget))
	UImage* SkillIcon;
	UPROPERTY(Meta = (BindWidget))
	UImage* CooldownImage;
	UPROPERTY(Meta = (BindWidget))
	UImage* Base;

	UPROPERTY(Meta = (BindWidget))
	class UButton* SkillButton;

	bool bDraggedOver;//标识当前是否正在被拖拽
	FLinearColor DragOverColor;//这个插槽为空的时候，有图标过来，显示的颜色
	FLinearColor DefaultColor;//默认颜色

	class UMaterialInstanceDynamic* CooldownMat;

	UPROPERTY(EditAnywhere, Category = DragDrop)
	TSubclassOf<UDragDropOperation> SkillDragOperation;


	bool bDeactived;

protected:
	UPROPERTY(BlueprintReadOnly)
	ABaseSkill* AssignedSpell;//分配的技能
	UPROPERTY(BlueprintReadOnly)
	FKey Key;//键盘按键，每一个SkillHotkey对应一个Key，用这个Key，即可释放SkillHotkey上的技能

	UFUNCTION()
	void OnSkillButtonClicked();

	void ResetStyle();//重设为初始的样子

	//只有当鼠标右键按下的时候，才来执行检测拖拽的方法
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//当检测到拖拽的时候调用的函数
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	//当拖拽着一个UI上晃荡的时候
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	//当拖拽着UI从一个UI上离开的时候触发
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	//当放下的时候
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	FORCEINLINE void SetKey(FKey KeyRef) { this->Key = KeyRef; }//设置Key
	FORCEINLINE FKey GetKey() { return Key; }
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void AssignSpell(ABaseSkill* NewAssigneSpell);//为该快捷键分配技能

	void ClearAssignedSpell();//清除分配的技能

	void EnableHotkey();//激活该快捷键，点击有效

	void DisableHotkey();//失活快捷键，当技能释放着的时候，就不能再有效了

	FORCEINLINE ABaseSkill* GetAssignedSpell() { return AssignedSpell; }
};

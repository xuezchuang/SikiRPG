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

	bool bDraggedOver;//��ʶ��ǰ�Ƿ����ڱ���ק
	FLinearColor DragOverColor;//������Ϊ�յ�ʱ����ͼ���������ʾ����ɫ
	FLinearColor DefaultColor;//Ĭ����ɫ

	class UMaterialInstanceDynamic* CooldownMat;

	UPROPERTY(EditAnywhere, Category = DragDrop)
	TSubclassOf<UDragDropOperation> SkillDragOperation;


	bool bDeactived;

protected:
	UPROPERTY(BlueprintReadOnly)
	ABaseSkill* AssignedSpell;//����ļ���
	UPROPERTY(BlueprintReadOnly)
	FKey Key;//���̰�����ÿһ��SkillHotkey��Ӧһ��Key�������Key�������ͷ�SkillHotkey�ϵļ���

	UFUNCTION()
	void OnSkillButtonClicked();

	void ResetStyle();//����Ϊ��ʼ������

	//ֻ�е�����Ҽ����µ�ʱ�򣬲���ִ�м����ק�ķ���
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//����⵽��ק��ʱ����õĺ���
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	//����ק��һ��UI�ϻε���ʱ��
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	//����ק��UI��һ��UI���뿪��ʱ�򴥷�
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	//�����µ�ʱ��
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	FORCEINLINE void SetKey(FKey KeyRef) { this->Key = KeyRef; }//����Key
	FORCEINLINE FKey GetKey() { return Key; }
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void AssignSpell(ABaseSkill* NewAssigneSpell);//Ϊ�ÿ�ݼ����似��

	void ClearAssignedSpell();//�������ļ���

	void EnableHotkey();//����ÿ�ݼ��������Ч

	void DisableHotkey();//ʧ���ݼ����������ͷ��ŵ�ʱ�򣬾Ͳ�������Ч��

	FORCEINLINE ABaseSkill* GetAssignedSpell() { return AssignedSpell; }
};

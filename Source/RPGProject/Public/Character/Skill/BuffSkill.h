// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/BaseSkill.h"
#include "BuffSkill.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API ABuffSkill : public ABaseSkill
{
	GENERATED_BODY()

		FTimerHandle TimerHandle_UpdateEffect;

public:
	class UBuffWidget* BuffWidgetRef;

	UPROPERTY(EditAnywhere)
	class UTexture2D* BuffIcon;

	float BuffDuration;//Buff�ӳֵ�ʱ��
	float RemainingTime;

	virtual void ApplyEffect();//BuffЧ��ʩ��
	virtual void RemoveEffect();//BuffЧ���Ƴ�

	virtual void ResetBuff();//����BuffЧ��

	virtual void OnActive();//Buff���������ܵ���ڣ�����AppklayEffect֮�⣬��Ҫ������Buff֮�������
	virtual void OnDeActive();

	virtual void OnUpdateBuff();//�ڶ�ʱ�������ĺ���

	virtual void OnSkillNotify() override;
	
};

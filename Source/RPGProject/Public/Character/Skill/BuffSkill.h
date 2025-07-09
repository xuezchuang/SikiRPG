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

	float BuffDuration;//Buff加持的时间
	float RemainingTime;

	virtual void ApplyEffect();//Buff效果施加
	virtual void RemoveEffect();//Buff效果移除

	virtual void ResetBuff();//重置Buff效果

	virtual void OnActive();//Buff激活，这个技能的入口，出了AppklayEffect之外，还要完成添加Buff之类的事情
	virtual void OnDeActive();

	virtual void OnUpdateBuff();//在定时器持续的函数

	virtual void OnSkillNotify() override;
	
};

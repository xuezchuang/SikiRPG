// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/BaseSkill.h"
#include "MissileSkill.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AMissileSkill : public ABaseSkill
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Category=Skill)
	TSubclassOf<class AMagicProjectile> MagicProjectile;

	FTimerHandle TimerHandle_Caculate;

	bool BInAttackRange();

	virtual void OnTryCastSpell() override;
	
	virtual void InitializeSpellCast() override;//��ʼ�ͷţ����ж�ħ���Ƿ��㹻

	virtual void OnSpellCast() override;//���������ͷŵ�ʱ�򣬱��粥���ͷż��ܵĶ���

	virtual void OnSkillNotify() override;

	void CaculateDistanceToEnemy();
	
public:
	UPROPERTY(EditAnywhere,Category=Effect)
	UParticleSystem* MissileEffect;//������ӵ������е���Ч
	UPROPERTY(EditAnywhere, Category = Effect)
	UParticleSystem*ImpactEffect;//�ӵ���Ŀ�������ŵ���Ч

	void StopCaculate();
};

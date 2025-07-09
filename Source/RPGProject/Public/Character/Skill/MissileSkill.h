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
	
	virtual void InitializeSpellCast() override;//初始释放，来判断魔法是否足够

	virtual void OnSpellCast() override;//技能正在释放的时候，比如播放释放技能的动画

	virtual void OnSkillNotify() override;

	void CaculateDistanceToEnemy();
	
public:
	UPROPERTY(EditAnywhere,Category=Effect)
	UParticleSystem* MissileEffect;//发射的子弹所持有的特效
	UPROPERTY(EditAnywhere, Category = Effect)
	UParticleSystem*ImpactEffect;//子弹打到目标所播放的特效

	void StopCaculate();
};

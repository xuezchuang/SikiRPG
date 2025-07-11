// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillEnum.generated.h"

//技能作用的对象
UENUM()
enum  class ETargetTypes:uint8
{
	Self,
	SelectedEnemy,
	SelectedArea,//AOE
	AreaAroundSelf,//自身周围
	Missile //比如元气弹
};

//伤害类型
UENUM()
enum class EDamageType :uint8 {  
	Magic,
	Physical
};

//效果，比如效果增加，和效果削弱
UENUM()
enum class EEffectiveness :uint8 {
	Effective,
	SuperEffective,
	LowEffective
};


UCLASS()
class RPGPROJECT_API USkillEnum : public UObject
{
	GENERATED_BODY()
	
};

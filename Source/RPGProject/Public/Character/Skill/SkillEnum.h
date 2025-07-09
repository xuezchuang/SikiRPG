// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillEnum.generated.h"

//�������õĶ���
UENUM()
enum  class ETargetTypes:uint8
{
	Self,
	SelectedEnemy,
	SelectedArea,//AOE
	AreaAroundSelf,//������Χ
	Missile //����Ԫ����
};

//�˺�����
UENUM()
enum class EDamageType :uint8 {  
	Magic,
	Physical
};

//Ч��������Ч�����ӣ���Ч������
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

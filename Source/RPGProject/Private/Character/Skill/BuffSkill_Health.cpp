// Fill out your copyright notice in the Description page of Project Settings.

#include "BuffSkill_Health.h"

void ABuffSkill_Health::ApplyEffect()
{
	PlayerReference->ChangeCurrentHp(0.1);
}

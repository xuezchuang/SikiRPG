// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Other/StaticLibrary.h"
#include "Public/Character/Skill/BaseElement.h"
#include "Public/Character/Skill/SkillEnum.h"
#include "Public/Enemy/NormalEnemy.h"

float UStaticLibrary::CaculateFinalDamage(float BaseDamage, int CritChance, TSubclassOf<ABaseElement> AttackerElement, TSubclassOf<ABaseElement> DefenderElement)
{
	float CurrentDamage = BaseDamage * FMath::RandRange(0.9f, 1.1f);
	EEffectiveness LocalEffecctiveness;
	if (AttackerElement&&DefenderElement) {
		if (DefenderElement->GetDefaultObject<ABaseElement>()->ElementInfo.Weakness.Contains(AttackerElement)) {
			LocalEffecctiveness = EEffectiveness::LowEffective;
		}
		else if (DefenderElement->GetDefaultObject<ABaseElement>()->ElementInfo.Enhance.Contains(AttackerElement)) {
			LocalEffecctiveness = EEffectiveness::SuperEffective;
		}
		else {
			LocalEffecctiveness = EEffectiveness::Effective;
		}
	}
	else {
		LocalEffecctiveness = EEffectiveness::Effective;
	}

	switch (LocalEffecctiveness)
	{
	case EEffectiveness::Effective:
		CurrentDamage = CurrentDamage * 1;
		break;
	case EEffectiveness::SuperEffective:
		CurrentDamage = CurrentDamage * 1.5f;
		break;
	case EEffectiveness::LowEffective:
		CurrentDamage = CurrentDamage * 0.7f;
		break;
	default:
		break;
	}

	if (FMath::RandHelper(101) <= CritChance) {
		CurrentDamage = CurrentDamage * 2.0f;
	}

	return CurrentDamage;
}

bool UStaticLibrary::BIsEnemy(AActor* Actor)
{
	return (Actor->GetClass() == ANormalEnemy::StaticClass() || Actor->GetClass()->IsChildOf(ANormalEnemy::StaticClass()));
}
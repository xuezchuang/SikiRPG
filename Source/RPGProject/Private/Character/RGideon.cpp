// Fill out your copyright notice in the Description page of Project Settings.

#include "RGideon.h"
#include "ConstructorHelpers.h"
#include "Animation/AnimInstance.h"



ARGideon::ARGideon()
{
	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("AnimBlueprint'/Game/Blueprints/Character/Animations/ABP_Gideon.ABP_Gideon_C'"));
	GetMesh()->SetAnimInstanceClass(Anim.Class);
}

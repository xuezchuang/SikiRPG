// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillDrag.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"



void USkillDrag::SetSkillTexture(class UTexture2D* Texture)
{
	SkillIcon->SetBrushFromTexture(Texture);
}

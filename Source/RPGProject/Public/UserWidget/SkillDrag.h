// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillDrag.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USkillDrag : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta=(BindWidget))
	class UImage* SkillIcon;

public:
	void SetSkillTexture(class UTexture2D* Texture);
	
};

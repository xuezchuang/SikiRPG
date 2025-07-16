// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuffWidget.generated.h"

/**
 *
 */
UCLASS()
class RPGPROJECT_API UBuffWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
	class UImage* BuffIcon;
	UPROPERTY(Meta = (BindWidget))
	class UImage* CooldownImage;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* CooldownTime;

	class UTexture2D* BuffTexture;

	class UMaterialInstanceDynamic* CooldownMat;

	virtual void NativeConstruct() override;


};

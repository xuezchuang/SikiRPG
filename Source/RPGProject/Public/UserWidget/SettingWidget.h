// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta = (BindWidget))
	class UButton* SaveButton;

	class ARBaseCharacter* PlayerCharacter;

	UFUNCTION()
	void OnSaveButtonClicked();

	virtual void NativeConstruct() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RPGSave.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FString SavedName;
	UPROPERTY()
	float SavedHP;
	UPROPERTY()
	float SavedMP;
	UPROPERTY()
	float SavedEXP;
	UPROPERTY()
	float SavedLevel;
	UPROPERTY()
	float SavedCoin;
};

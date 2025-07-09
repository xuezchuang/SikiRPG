// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemCoin.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/UserWidget/MainUserWidget.h"

void AItemCoin::OnIteractWith(class ARBaseCharacter* Character)
{
	Character->IncreaseCoin(Amount);
	Character->MainUserWidget->AddItemToObtainedQueue(this->GetClass(), Amount);
}

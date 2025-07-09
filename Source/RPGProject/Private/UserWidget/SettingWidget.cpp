// Fill out your copyright notice in the Description page of Project Settings.

#include "SettingWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Character/RBaseCharacter.h"
#include "Components/Button.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Kismet/GameplayStatics.h"
#include "Public/InveotorySystem/Storage.h"

void USettingWidget::OnSaveButtonClicked()
{
	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	}
	PlayerCharacter->SaveGame();

	if (PlayerCharacter && PlayerCharacter->InventoryRef)
	{
		PlayerCharacter->InventoryRef->SaveInventory();
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStorage::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		AStorage* Storage = Cast<AStorage>(Actor);
		if (Storage)
		{
			Storage->SaveStorage();
		}
	}
}

void USettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SaveButton->OnClicked.AddDynamic(this, &USettingWidget::OnSaveButtonClicked);
}

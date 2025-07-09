// Fill out your copyright notice in the Description page of Project Settings.

#include "Merchant.h"
#include "Public/UserWidget/Inventory/ShopWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/UserWidget/Inventory/SellWidget.h"
#include "MainUserWidget.h"

void AMerchant::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter= Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	ShopWidget = CreateWidget<UShopWidget>(GetWorld(), LoadClass<UShopWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Inventory/WBP_Shop.WBP_Shop_C'")));
	ShopWidget->AddToViewport(3);
	ShopWidget->SetDesiredSizeInViewport(FVector2D(500, 700));
	ShopWidget->SetAnchorsInViewport(FAnchors(0.5, 0.5, 0.5, 0.5));
	ShopWidget->SetAlignmentInViewport(FVector2D(0.5, 0.5));
	ShopWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMerchant::OnIteractWith(class ARBaseCharacter* Character)
{
	if (!bShopOpen)
	{
		ShopWidget->Merchant = this;
		ShopWidget->PlayerCharacter = PlayerCharacter;
		ShopWidget->SellWidget->InventoryRef = PlayerCharacter->InventoryRef;
		ShopWidget->SellWidget->ShopWidget = ShopWidget;
		ShopWidget->UpdateCoin();
		ShopWidget->GenerateItemList();
		ShopWidget->SetVisibility(ESlateVisibility::Visible);
		if(Character->MainUserWidget&&!Character->MainUserWidget->ShopWidget)
		Character->MainUserWidget->ShopWidget = ShopWidget;
		bShopOpen = true;

	}
	else {
		ShopWidget->SetVisibility(ESlateVisibility::Hidden);
		bShopOpen = false;
	}
}

void AMerchant::OnLeavePlayerRadius(class ARBaseCharacter* Character)
{
	ShopWidget->SetVisibility(ESlateVisibility::Hidden);
	bShopOpen = false;
}

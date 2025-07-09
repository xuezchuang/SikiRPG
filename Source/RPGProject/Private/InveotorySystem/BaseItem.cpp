// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/InveotorySystem/BaseItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "ConstructorHelpers.h"
#include "Public/UserWidget/Quest/InteractionWidget.h"
#include "Components/TextBlock.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Public/UserWidget/MainUserWidget.h"

// Sets default values
ABaseItem::ABaseItem()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetGenerateOverlapEvents(false);
	RootComponent = StaticMeshComp;
	BoxCollisonComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisonComp "));
	BoxCollisonComp->SetupAttachment(RootComponent);


	InterationWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InterationWidgetComp"));
	InterationWidgetComp->SetupAttachment(RootComponent);

	InterationWidgetComp->SetRelativeScale3D(FVector(0.7, 0.7, 0.7));
	InterationWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	InterationWidgetComp->SetCollisionProfileName(TEXT("NoCollision"));
	InterationWidgetComp->SetGenerateOverlapEvents(false);
	InterationWidgetComp->SetVisibility(false);

	static ConstructorHelpers::FClassFinder<UUserWidget> US(TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Quest/WBP_Interaction.WBP_Interaction_C'"));
	InterationWidgetComp->SetWidgetClass(US.Class);
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	Cast<UInteractionWidget>(InterationWidgetComp->GetUserWidgetObject())->NameText->SetText(ItemInfo.Name);
	Cast<UInteractionWidget>(InterationWidgetComp->GetUserWidgetObject())->InteractionText->SetText(FText::FromString("Press [F] to pick up!"));
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseItem::OnEnterPlayerRadius(class ARBaseCharacter* Character)
{
	InterationWidgetComp->SetVisibility(true);
}

void ABaseItem::OnLeavePlayerRadius(class ARBaseCharacter* Character)
{
	InterationWidgetComp->SetVisibility(false);
}

void ABaseItem::OnIteractWith(class ARBaseCharacter* Character)
{
	int RestAmount = Character->InventoryRef->AddItem(this->GetClass(),Amount);
	if (Amount != RestAmount) {
		Character->MainUserWidget->AddItemToObtainedQueue(this->GetClass(), Amount - RestAmount);
	}
	if (RestAmount > 0) {
		Amount = RestAmount;
		if (ID != -1) {
			Character->InventoryRef->LootedPickups.Add(FSavedPickup{ ID,RestAmount });
		}
	}
	else {
		if (ID != -1) {
			Character->InventoryRef->LootedPickups.Add(FSavedPickup{ ID,0 });
		}
		Destroy();
	}

}

void ABaseItem::OnUsed()
{
	if (InventoryRef->RemoveItemAtIndex(Index, 1)) {
		UE_LOG(LogTemp, Warning, TEXT("You used the %s"), *ItemInfo.Name.ToString());
		Destroy();
	}
}


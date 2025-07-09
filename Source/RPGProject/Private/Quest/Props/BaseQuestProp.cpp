// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Quest/Props/BaseQuestProp.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "ConstructorHelpers.h"
#include "Public/UserWidget/Quest/InteractionWidget.h"
#include "Components/TextBlock.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/Quest/QuestManager.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABaseQuestProp::ABaseQuestProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(RootComponent);
	InteractionWidget->SetRelativeScale3D(FVector(0.1,0.1,0.1));
	InteractionWidget->SetRelativeRotation(FRotator(0, 90, 0));

	InteractionWidget->SetCollisionProfileName(TEXT("NoCollision"));
	InteractionWidget->SetGenerateOverlapEvents(false);
	InteractionWidget->SetVisibility(false);
	static ConstructorHelpers::FClassFinder<UUserWidget> US(TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Quest/WBP_Interaction.WBP_Interaction_C'"));
	InteractionWidget->SetWidgetClass(US.Class);

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	ParticleSystemComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseQuestProp::BeginPlay()
{
	Super::BeginPlay();
	Cast<UInteractionWidget>(InteractionWidget->GetUserWidgetObject())->NameText->SetText(Name);
	Cast<UInteractionWidget>(InteractionWidget->GetUserWidgetObject())->InteractionText->SetText(FText::FromString("Press [F] to pick up"));
}

// Called every frame
void ABaseQuestProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseQuestProp::OnEnterPlayerRadius(class ARBaseCharacter* Character)
{
	InteractionWidget->SetVisibility(true);
}

void ABaseQuestProp::OnLeavePlayerRadius(class ARBaseCharacter* Character)
{
	InteractionWidget->SetVisibility(false);
}

void ABaseQuestProp::OnIteractWith(class ARBaseCharacter* Character)
{
	Character->ObtainedProps.Add(this->GetClass());
	Character->QuestManager->OnObjectFound(this->GetClass());
	Destroy();
}


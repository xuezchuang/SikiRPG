// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/NPC/BaseNPC.h"
#include "Components/WidgetComponent.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/Quest/QuestManager.h"
#include "Public/Quest/BaseQuest.h"
#include "Public/UserWidget/Quest/InteractionWidget.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "Public/UserWidget/Quest/MessageWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "PaperSprite.h"
#include "TimerManager.h"

// Sets default values
ABaseNPC::ABaseNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(RootComponent);
	InteractionWidget->SetVisibility(false);
	InteractionWidget->SetCollisionProfileName(TEXT("NoCollision"));
	InteractionWidget->SetGenerateOverlapEvents(false);
	InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> IW(TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Quest/WBP_Interaction.WBP_Interaction_C'"));
	InteractionWidget->SetWidgetClass(IW.Class);

	MessageWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("MessageWidgetComp"));
	MessageWidgetComp->SetupAttachment(RootComponent);
	MessageWidgetComp->SetVisibility(false);
	MessageWidgetComp->SetCollisionProfileName(TEXT("NoCollision"));
	MessageWidgetComp->SetGenerateOverlapEvents(false);
	MessageWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> MW(TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Quest/WBP_Message.WBP_Message_C'"));
	MessageWidgetComp->SetWidgetClass(MW.Class);

	QuestIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("QuestIcon"));
	QuestIcon->SetupAttachment(RootComponent);
	QuestIcon->SetRelativeLocation(FVector(0, 0, 150.0f));

	QuestIcon->SetRelativeScale3D(FVector(0.7, 0.7, 0.7));
	QuestIcon->SetSpriteColor(FLinearColor::Yellow);
	QuestIcon->SetCollisionProfileName(TEXT("NoCollision"));
	QuestIcon->SetGenerateOverlapEvents(false);

	static ConstructorHelpers::FObjectFinder<UPaperSprite> QuesSprite(TEXT("PaperSprite'/Game/Textures/QuestTextures/Quest_Icon_Sprite.Quest_Icon_Sprite'"));
	QuestIcon->SetSprite(QuesSprite.Object);

	GetMesh()->SetRelativeLocation(FVector(0,0,-88));
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
}

// Called when the game starts or when spawned
void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
	QuestIcon->SetWorldRotation(FRotator(0,88.5,-90));
	Cast<UInteractionWidget>(InteractionWidget->GetUserWidgetObject())->SetNameText(Name);
	QuestIcon->SetVisibility(bHasQuest);
	SetOwner(Cast<AActor>(UGameplayStatics::GetPlayerCharacter(this, 0))) ;
	QuestIcon->SetOwnerNoSee(true);

	MessageWidget =Cast<UMessageWidget>(MessageWidgetComp->GetUserWidgetObject());
}

void ABaseNPC::ResetMessage()
{
	MessageWidgetComp->SetVisibility(false);
	InteractionWidget->SetVisibility(bInPlayerRadius);
	bCanTalkTo = true;
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetMessage);
}

// Called every frame
void ABaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseNPC::OnEnterPlayerRadius(class ARBaseCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("OnEnterPlayerRadius"));
	bInPlayerRadius = true;
	if(bCanTalkTo)
	InteractionWidget->SetVisibility(true);
}

void ABaseNPC::OnLeavePlayerRadius(class ARBaseCharacter* Character)
{
	bInPlayerRadius = false;
	InteractionWidget->SetVisibility(false);
}

void ABaseNPC::OnIteractWith(class ARBaseCharacter* Character)
{
	if (MyQuest)
	{
		if (!Character->QuestManager->AllQuestClasses.Contains(MyQuest)) {
			Character->QuestManager->AddNewQuest(MyQuest);
			QuestIcon->SetVisibility(false);
		}
		else {
			OnTalkWith(Character);
		}
	}
	else {
		OnTalkWith(Character);
	}
}

void ABaseNPC::OnTalkWith(class ARBaseCharacter* Character)
{
	ShowMessage(Message, DefaultDuration, Character);
}

void ABaseNPC::ShowMessage(FText MessageRef, float Duration, class ARBaseCharacter* Character)
{
	if (bCanTalkTo) {
		InteractionWidget->SetVisibility(false);
		MessageWidget->SetMessage(MessageRef);
		MessageWidgetComp->SetVisibility(true);
		bCanTalkTo = false;
		Character->QuestManager->OnTalkToNPC(this->GetClass(), NPCId);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetMessage,this,&ABaseNPC::ResetMessage,Duration,false);
	}
}


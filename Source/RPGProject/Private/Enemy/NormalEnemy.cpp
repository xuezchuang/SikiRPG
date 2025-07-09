// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Enemy/NormalEnemy.h"
#include "Public/Enemy/NormalEnemyController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Public/Character/RBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "RPGProject.h"
#include "WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Public/UserWidget/EnemyInfoWidget.h"
#include "Components/SphereComponent.h"
#include "Math/Color.h"
#include "Components/TextBlock.h"
#include "Public/UserWidget/EnemyInfoWidget.h"
#include "Components/ProgressBar.h"
#include "Public/Other/StaticLibrary.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Components/Border.h"
#include "Components/ArrowComponent.h"
#include "Public/Quest/QuestManager.h"

#define LOCTEXT_NAMESPACE "EnemyNameSpace"

// Sets default values
ANormalEnemy::ANormalEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ANormalEnemyController::StaticClass();
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	if (AIPerceptionComp)
	{
		SightConfig->SightRadius = 800;
		SightConfig->LoseSightRadius = 2000;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	}

	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->SetDominantSense(UAISense_Sight::StaticClass());

	AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &ANormalEnemy::OnSightPerceptionUpdate);
	EnemyWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyWidgetComp"));

	static ConstructorHelpers::FClassFinder<UEnemyInfoWidget> EIW(TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/Enemy/WBP_EnemyInfo.WBP_EnemyInfo_C'"));
	EnemyWidgetComp->SetWidgetClass(EIW.Class);
	EnemyWidgetComp->SetDrawSize(FVector2D(200.0f, 60.0f));
	EnemyWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyWidgetComp->SetTwoSided(true);
	EnemyWidgetComp->SetVisibility(false);

	EnemyWidgetComp->SetupAttachment(RootComponent);
	EnemyWidgetComp->SetRelativeLocation(FVector(0, 0, 100));


	ShowUICollision = CreateDefaultSubobject<USphereComponent>(TEXT("ShowUICollison"));
	ShowUICollision->SetupAttachment(RootComponent);
	ShowUICollision->OnComponentBeginOverlap.AddDynamic(this, &ANormalEnemy::OnBeginOverlap);
	ShowUICollision->OnComponentEndOverlap.AddDynamic(this, &ANormalEnemy::OnEndOverlap);
	HitArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("HitArrow"));
	HitArrow->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyInfoWidget = Cast<UEnemyInfoWidget>(EnemyWidgetComp->GetUserWidgetObject());
	MyController = Cast<ANormalEnemyController>(GetController());
	if (MyController)
		MyController->Patrol();
	StartLocation = GetActorLocation();
	EnemyWidgetComp->SetVisibility(bInShowRange);
	InitWidgetText();
	CurrentHealth = TotalHealth;
	UpdateHealthBar();
}

void ANormalEnemy::OnSightPerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	if (bAggressive && !MyController->bWasAggroed)
		for (auto Actor : UpdatedActors)
			if (Cast<ARBaseCharacter>(Actor))
			{
				UE_LOG(LogTemp, Warning, TEXT("OnSightPerception"));
				MyController->OnAggroedPulled(Cast<ARBaseCharacter>(Actor));
			}
}

void ANormalEnemy::NotifyHit()
{
	AttackRay();
}

void ANormalEnemy::AttackRay()
{
	int Range = 250;
	StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + GetCapsuleComponent()->GetForwardVector() * Range;

	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, EnemyHitChannel, QueryParams))
	{
		ARBaseCharacter* Character = Cast<ARBaseCharacter>(HitResult.GetActor());
		if (Character)
		{
			IDamageableInterface* DamageableInterface = Cast<IDamageableInterface>(Character);
			if (DamageableInterface)
			{
				DamageableInterface->OnReceiveDamage(BaseDamage, DamageType, Element, CritChance, this, nullptr);
			}
		}
	}
}

void ANormalEnemy::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bDead)
	{
		if (Cast<ARBaseCharacter>(OtherActor) && !EnemyWidgetComp->IsVisible())
		{
			bInShowRange = true;
			EnemyWidgetComp->SetVisibility(true);
		}
	}
}

void ANormalEnemy::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bDead)
	{
		bInShowRange = false;
		if (Cast<ARBaseCharacter>(OtherActor) && EnemyWidgetComp->IsVisible())
		{
			EnemyWidgetComp->SetVisibility(false);
		}
	}
}

void ANormalEnemy::InitWidgetText()
{
	EnemyInfoWidget->LevelName->SetText(FText::Format(LOCTEXT("EnemyNameSpace", "[Lv{0}.{1}]"), FText::AsNumber(Level), Name));
	FLinearColor CurrentColor;
	if (bAggressive)
	{
		CurrentColor = FLinearColor::Red;
	}
	else
	{
		CurrentColor = FLinearColor::White;
	}
	EnemyInfoWidget->SetColorAndOpacity(CurrentColor);
}

void ANormalEnemy::OnReceiveDamage(float BaseDamageRef, EDamageType Type, TSubclassOf<ABaseElement> ElementRef, int CritChanceRef, AActor* Attacker, ABaseSkill* Spell)
{
	if (!bDead && !UStaticLibrary::BIsEnemy(Attacker))
	{
		CurrentHealth = FMath::Clamp((CurrentHealth - UStaticLibrary::CaculateFinalDamage(BaseDamageRef, CritChanceRef, ElementRef, this->Element)), 0.0f, TotalHealth);
		UpdateHealthBar();
		if (CurrentHealth <= 0.5f)
		{
			OnDeath(Attacker);
		}
		else
		{
			MyController->OnAggroedPulled(Attacker);
		}
	}
}

void ANormalEnemy::OnDeath(AActor* Killer)
{
	MyController->AnimInstance->Montage_Stop(0.0f);
	bDead = true;
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	EnemyWidgetComp->SetVisibility(false);
	ARBaseCharacter* TempCharacter = Cast<ARBaseCharacter>(Killer);
	if (TempCharacter)
	{
		TempCharacter->IncreaseCurrentExp(ExpForKill);
		if (TempCharacter->SelectedEnemy == this)
		{
			TempCharacter->SelectedEnemy = nullptr;
			TempCharacter->MainUserWidget->EnemyBorder->SetVisibility(ESlateVisibility::Hidden);

		}
		TempCharacter->QuestManager->OnEnemyKilled(this->GetClass());
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorldTimerManager().SetTimer(TimerHandle_DelayDeath, this, &ANormalEnemy::OnDelayDeath, 2.0f, false);

}


void ANormalEnemy::OnDelayDeath()
{
	SetActorHiddenInGame(true);
	if (bDoesRespawn)
		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ANormalEnemy::Respawn, RespawnTime, false);
	else
	{
		Destroy();
	}
}

void ANormalEnemy::Respawn()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bDead = false;
	SetActorLocation(StartLocation);
	CurrentHealth = TotalHealth;
	UpdateHealthBar();
	InitWidgetText();
	MyController->bIsPatrolling = true;
	MyController->bWasAggroed = false;
	MyController->TargetActor = nullptr;
	MyController->CurrentAttackIndex = 0;
	MyController->bIsRunningBack = false;
	GetCharacterMovement()->MaxWalkSpeed = MyController->PatrolWalkSpeed;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	EnemyWidgetComp->SetVisibility(bInShowRange);
	SetActorHiddenInGame(true);
	MyController->Patrol();
}

void ANormalEnemy::OnSelected(class ARBaseCharacter* Character)
{
	if (!bDead && !bSelected)
	{
		SelectingCharacter = Character;
		bSelected = true;
		GetMesh()->SetRenderCustomDepth(true);
		Character->MainUserWidget->EnemyNameLevel->SetText(FText::Format(LOCTEXT("EnemyNameSpace", "{0}(Lv.{1})"), Name, FText::AsNumber(Level)));
		UpdateHealthBar();
		Character->MainUserWidget->EnemyBorder->SetVisibility(ESlateVisibility::Visible);
		Character->SelectedEnemy = this;
	}
}

void ANormalEnemy::OnSelectionEnd(class ARBaseCharacter* Character)
{
	if (!bDead && bSelected)
	{
		GetMesh()->SetRenderCustomDepth(false);
		bSelected = false;
		Character->MainUserWidget->EnemyBorder->SetVisibility(ESlateVisibility::Hidden);
		SelectingCharacter = nullptr;
		Character->SelectedEnemy = nullptr;
	}
}


void ANormalEnemy::UpdateHealthBar()
{
	EnemyInfoWidget->HealthBar->SetPercent(CurrentHealth / TotalHealth);
	if (bSelected)
	{
		SelectingCharacter->MainUserWidget->EnemyHpBar->SetPercent(CurrentHealth / TotalHealth);
		SelectingCharacter->MainUserWidget->EnemyHpText->SetText(FText::Format(LOCTEXT("EnemyNameSpace", "{0}/{1}"), FText::AsNumber(FMath::RoundHalfToZero(CurrentHealth)), FText::AsNumber(TotalHealth)));
	}
}

// Called every frame
void ANormalEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANormalEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

#undef LOCTEXT_NAMESPACE
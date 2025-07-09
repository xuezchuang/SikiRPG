// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Other/DamageArea.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Public/Character/RBaseCharacter.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADamageArea::ADamageArea()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ADamageArea::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this,&ADamageArea::OnEndOverlap);
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(RootComponent);
}


void ADamageArea::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ARBaseCharacter>(OtherActor))
	{
		Player = Cast<ARBaseCharacter>(OtherActor);
		GetWorldTimerManager().SetTimer(TimerHandle_Damage,this,&ADamageArea::ApplayAreaDanmage,1.0f,true);
	}
}

void ADamageArea::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ARBaseCharacter>(OtherActor))
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Damage);
	}

}

void ADamageArea::ApplayAreaDanmage()
{
	UGameplayStatics::ApplyDamage(Player, 5.0f, nullptr, this, nullptr);
}


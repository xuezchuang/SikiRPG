// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Character/MiniCam.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Character/RBaseCharacter.h"


AMiniCam::AMiniCam()
{
	GetCaptureComponent2D()->SetRelativeRotation(FRotator(-90.0f,0.0f,0.0f));
}

void AMiniCam::BeginPlay()
{
	Super::BeginPlay();
	Character=Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	SetActorLocation(FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, Character->GetActorLocation().Z+1200.0f));
}

void AMiniCam::OutsideTick()
{
	//FVector TargetLocation = FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, Character->GetActorLocation().Z+1200);
	FVector TargetLocation = Character->GetActorLocation() + FVector(0, 0, 1200.0f);
	SetActorLocation(TargetLocation);
}

void AMiniCam::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	OutsideTick();
}

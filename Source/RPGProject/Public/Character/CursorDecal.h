// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CursorDecal.generated.h"

UCLASS()
class RPGPROJECT_API ACursorDecal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACursorDecal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UDecalComponent* CursorToWorld;
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* DefaultRootComponent;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/*UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);*/
	
};

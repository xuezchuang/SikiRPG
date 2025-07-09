// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicProjectile.generated.h"

class UParticleSystem;

UCLASS()
class RPGPROJECT_API AMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicProjectile();

protected:

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollsion;
	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ParticleSystem;
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(VisibleAnywhere,Category=Effect)
	UParticleSystem* MissileEffect;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* ImpactEffect;

	class AMissileSkill* Skill;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};

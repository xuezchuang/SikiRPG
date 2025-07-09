// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Public/Character/Skill/MissileSkill.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/Enemy/NormalEnemy.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Public/Interface/DamageableInterface.h"

// Sets default values
AMagicProjectile::AMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereCollsion = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollsion->OnComponentBeginOverlap.AddDynamic(this,&AMagicProjectile::OnOverlap);
	RootComponent = SphereCollsion;

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->ProjectileGravityScale = 0;
	ProjectileMovementComponent->bIsHomingProjectile = true;
}

// Called when the game starts or when spawned
void AMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	Skill = Cast<AMissileSkill>(GetOwner());
	MissileEffect = Skill->MissileEffect;
	ImpactEffect = Skill->ImpactEffect;
	ParticleSystem->SetTemplate(MissileEffect);
	/*ProjectileMovementComponent->InitialSpeed = Skill->GetCurrentStage().MisiileSpeed;*/

	ProjectileMovementComponent->HomingTargetComponent = Skill->GetPlayerReference()->SelectedEnemy->HitArrow;
	ProjectileMovementComponent->HomingAccelerationMagnitude = Skill->GetCurrentStage().MisiileSpeed;
}

void AMagicProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ANormalEnemy* TempEnemy=Cast<ANormalEnemy>(OtherActor);
	if (TempEnemy&&OtherComp==TempEnemy->GetCapsuleComponent()) {
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect, FVector(SweepResult.ImpactPoint),FRotator::ZeroRotator);

		IDamageableInterface* DamageableInterface= Cast<IDamageableInterface>(TempEnemy);
		if (DamageableInterface) {
			DamageableInterface->OnReceiveDamage(Skill->GetCurrentStage().Damage, Skill->GetCurrentStage().DamageType, Skill->GetSkillInfo().Element, Skill->GetCurrentStage().CriticalChance, Skill->GetPlayerReference(), Skill);
		}
		Destroy();
	}
}

// Called every frame
void AMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


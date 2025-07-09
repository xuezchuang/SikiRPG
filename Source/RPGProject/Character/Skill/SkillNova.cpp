// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillNova.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Character/RBaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Public/Enemy/NormalEnemy.h"
#include "Public/Interface/DamageableInterface.h"
#include "RPGProject.h"
#include "Kismet/KismetSystemLibrary.h"

void ASkillNova::OnSkillNotify()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, PlayerReference->GetMesh()->GetSocketLocation("foot_l"), PlayerReference->GetActorRotation());

	TArray<FHitResult> HitResults;

	FCollisionShape CollisonSphere = FCollisionShape::MakeSphere(GetCurrentStage().Range);

	if (GetWorld()->SweepMultiByChannel(HitResults, PlayerReference->GetActorLocation(), PlayerReference->GetActorLocation(), FQuat::Identity, ECC_Visibility, CollisonSphere))
	{
		//UKismetSystemLibrary::DrawDebugSphere(this, PlayerReference->GetActorLocation(), GetCurrentStage().Range,12,FLinearColor::Red, 3.0f,2);
		for (auto HitResult : HitResults)
		{
			if (HitResult.GetActor())
			{
				ANormalEnemy* Enemy = Cast<ANormalEnemy>(HitResult.GetActor());
				if (Enemy)
				{
					UE_LOG(LogTemp, Warning, TEXT("HitEnemy"));
					IDamageableInterface* DamagebaleInterface = Cast<IDamageableInterface>(Enemy);
					if (DamagebaleInterface)
					{
						DamagebaleInterface->OnReceiveDamage(GetCurrentStage().Damage, GetCurrentStage().DamageType, GetSkillInfo().Element, GetCurrentStage().CriticalChance, PlayerReference, this);
					}
				}
			}
		}
	}

}

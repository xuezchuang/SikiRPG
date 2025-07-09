// Fill out your copyright notice in the Description page of Project Settings.

#include "MissileSkill.h"
#include "Public/Character/RBaseCharacter.h"
#include "Public/Enemy/NormalEnemy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/Character/Skill/MagicProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"

bool AMissileSkill::BInAttackRange()
{
	return (PlayerReference->GetDistanceTo(PlayerReference->SelectedEnemy) <= GetCurrentStage().Range);
}

void AMissileSkill::OnTryCastSpell()
{
	if (PlayerReference->SelectedEnemy) {
		Super::OnTryCastSpell();
	}
}

void AMissileSkill::InitializeSpellCast()
{
	if (BInAttackRange()) {
		Super::InitializeSpellCast();
	}
	else {
		GetWorldTimerManager().SetTimer(TimerHandle_Caculate,this,&AMissileSkill::CaculateDistanceToEnemy,0.1f,true);
		PlayerReference->MissileSkill = this;

	}
}

void AMissileSkill::OnSpellCast()
{
	PlayerReference->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerReference->GetActorLocation(), PlayerReference->SelectedEnemy->GetActorLocation()));
	Super::OnSpellCast();
}

void AMissileSkill::OnSkillNotify()
{
	UE_LOG(LogTemp,Warning,TEXT("AMissileSkill::OnSkillNotify()"));
	FActorSpawnParameters Params;
	Params.Owner = this;
	GetWorld()->SpawnActor<AMagicProjectile>(MagicProjectile, PlayerReference->GetMesh()->GetSocketLocation("Muzzle_01"), PlayerReference->GetMesh()->GetSocketRotation("Muzzle_01"),Params);
}

void AMissileSkill::CaculateDistanceToEnemy()
{
	if (PlayerReference->SelectedEnemy) {
		if (BInAttackRange()) {
			InitializeSpellCast();
			StopCaculate();
		}
		else {
			UAIBlueprintHelperLibrary::SimpleMoveToActor(Cast<AController>(PlayerReference->PC) ,PlayerReference->SelectedEnemy);
		}
	}
	else {
		GetWorldTimerManager().ClearTimer(TimerHandle_Caculate);
	}
}

void AMissileSkill::StopCaculate()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Caculate);
	PlayerReference->MissileSkill = nullptr;
}

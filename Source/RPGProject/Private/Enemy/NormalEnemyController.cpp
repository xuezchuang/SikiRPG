// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Enemy/NormalEnemyController.h"
#include "Public/Enemy/NormalEnemy.h"
#include "NavigationSystem.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

ANormalEnemyController::ANormalEnemyController()
{
	NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
}


void ANormalEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyPawn = Cast<ANormalEnemy>(InPawn);

	AnimInstance = EnemyPawn->GetMesh()->GetAnimInstance();

	EnemyPawn->GetCharacterMovement()->MaxWalkSpeed = PatrolWalkSpeed;
}


int ANormalEnemyController::GetNextAnimationIndex()
{
	if (CurrentAttackIndex + 1 >= EnemyPawn->AttackAnimaions.Num()) {
		return 0;
	}
	else {
		return (CurrentAttackIndex + 1);
	}
}

bool ANormalEnemyController::BInAttackRange()
{
	if (TargetActor) {
		
		return(EnemyPawn->GetDistanceTo(TargetActor) <= AttackRange);
	}
	else
	{
		return false;
	}
}

void ANormalEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{

	if (bIsRunningBack) {

		bIsRunningBack = false;
		bIsPatrolling = true;
		EnemyPawn->GetCharacterMovement()->MaxWalkSpeed = PatrolWalkSpeed;
	}

	if (BInAttackRange()) {
		PerformAttack();
		return;
	}

	if (bIsPatrolling) {
		float ThinkTime = FMath::RandRange(1, 4);
		GetWorldTimerManager().SetTimer(TimerHandle_Patrol,this,&ANormalEnemyController::DetectedPatrol,ThinkTime,false);
	}
}

void ANormalEnemyController::DetectedPatrol()
{
	if (bIsPatrolling) {
		Patrol();
	}
}

void ANormalEnemyController::PerformAttack()
{
	if (!EnemyPawn->GetBDead() && !bIsRunningBack&&bWasAggroed) {

		bIsPatrolling = false;

		EnemyPawn->GetCharacterMovement()->StopMovementImmediately();

		if (TargetActor) {
			FRotator Rotation=UKismetMathLibrary::FindLookAtRotation(EnemyPawn->GetActorLocation(),TargetActor->GetActorLocation());
			EnemyPawn->SetActorRotation(Rotation);
		}

		CurrentAttackMontage = EnemyPawn->AttackAnimaions[CurrentAttackIndex];

		AnimInstance->Montage_Play(CurrentAttackMontage);

		GetWorldTimerManager().SetTimer(TimerHandle_AnimPlayOver,this,&ANormalEnemyController::OnAnimPlayOver,CurrentAttackMontage->GetPlayLength(),false);
	}
}

void ANormalEnemyController::OnAnimPlayOver()
{
	CurrentAttackMontage = nullptr;
	CurrentAttackIndex = GetNextAnimationIndex();

	if (BInAttackRange()) {
		PerformAttack();
	}
	else {
		MoveToActor(TargetActor);
	}
}

void ANormalEnemyController::OnAggroedPulled(AActor* Target)
{
	if (!bWasAggroed && !bIsRunningBack) {
		bWasAggroed = true;
		TargetActor = Target;
		bIsPatrolling = false;

		EnemyPawn->GetCharacterMovement()->MaxWalkSpeed = AggroedWalkSpeed;

		GetWorldTimerManager().SetTimer(TimerHnadle_CalDis,this,&ANormalEnemyController::CacualteTargetDistance,1.0f,true);

		if (BInAttackRange()) {
			PerformAttack();
		}
		else {
			MoveToActor(TargetActor);
		}
	}
}

void ANormalEnemyController::CacualteTargetDistance()
{
	if(TargetActor)
	if (FVector::Dist(EnemyPawn->GetActorLocation(), TargetActor->GetActorLocation())>MaxDistanceToFollowTarget) {
		OnReset();
	}
}

void ANormalEnemyController::OnReset()
{
	AnimInstance->Montage_Stop(0.0f);
	bIsRunningBack = true;
	EnemyPawn->CurrentHealth = EnemyPawn->TotalHealth;
	EnemyPawn->UpdateHealthBar();
	CurrentAttackIndex = 0;
	GetWorldTimerManager().ClearTimer(TimerHandle_Patrol);
	GetWorldTimerManager().ClearTimer(TimerHandle_AnimPlayOver);
	GetWorldTimerManager().ClearTimer(TimerHnadle_CalDis);
	TargetActor = nullptr;
	bWasAggroed = false;
	MoveToLocation(EnemyPawn->StartLocation);
}

void ANormalEnemyController::Patrol()
{
	const float SearchRadius = 1000.0f;
	if (NavSys) {
		FNavLocation RandomPt;
		bool bFound=NavSys->GetRandomReachablePointInRadius(EnemyPawn->GetActorLocation(), SearchRadius, RandomPt);
		if (bFound) {
			MoveToLocation(RandomPt.Location);
		}
	}
}

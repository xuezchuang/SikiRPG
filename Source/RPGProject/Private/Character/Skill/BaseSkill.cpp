// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Character/Skill/BaseSkill.h"
#include "SkillHotkey.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "RBaseCharacter.h"
#include "MainUserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "GameFramework//CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseSkill::ABaseSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseSkill::BeginPlay()
{
	Super::BeginPlay();
	PlayerReference = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	AnimInstance = PlayerReference->GetMesh()->GetAnimInstance();
}

void ABaseSkill::InitializeSpellCast()
{
	if (PlayerReference->GetCurrentMp() >= GetCurrentStage().ManaCost) {
		bCurrentCasting = true;
		PlayerReference->BeginSpellCast(this);
		PlayerReference->ChangeCurrentMp(GetCurrentStage().ManaCost*-1);
		OnSpellCast();
	}
	else {
		PlayerReference->MainUserWidget->PlayPopupAnimation();
	}
}

void ABaseSkill::OnSpellCast()
{
	UE_LOG(LogTemp,Warning,TEXT("Casting skill: %s"),*SkillInfo.Name.ToString());
	if (SkillAniamMontage) {
		PlaySkillAnimation(SkillAniamMontage);
	}
	OnCastCompleted();
}

void ABaseSkill::OnCastCompleted()
{
	PlayerReference->EndSpellCast(this);
	bCurrentCasting = false;
	bOnCooldown = true;

	if (Hotkey) {
		Hotkey->SkillButton->SetIsEnabled(false);
		Hotkey->SkillIcon->SetColorAndOpacity(FLinearColor(0.33, 0.33, 0.33, 1));
		Hotkey->CooldownMat->SetScalarParameterValue("Percent", 1);
		Hotkey->CooldownImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Hotkey->CooldownText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CurrentCD = GetCurrentStage().CooldownTime;
		GetWorldTimerManager().SetTimer(TimerHandle_Cooldown,this,&ABaseSkill::OnCooldown,0.01f,true);
	}
}

void ABaseSkill::OnCooldown()
{
	if (CurrentCD <= 0) {
		GetWorldTimerManager().ClearTimer(TimerHandle_Cooldown);
		OnCooldownExpired();
	}
	else {
		CurrentCD -= 0.01f;
		Hotkey->CooldownMat->SetScalarParameterValue("Percent", CurrentCD / GetCurrentStage().CooldownTime);
		if(CurrentCD>=1.0f)
		Hotkey->CooldownText->SetText(FText::AsNumber(FMath::RoundHalfToZero(CurrentCD)));
		else {
			Hotkey->CooldownText->SetText(FText::AsNumber(FMath::RoundHalfToZero(10.0f*CurrentCD)/10));
		}
	}
	
}

void ABaseSkill::OnCooldownExpired()
{
	bOnCooldown = false;
	if (Hotkey) {
		if (!Hotkey->bDeactived) {
			Hotkey->SkillButton->SetIsEnabled(true);
			Hotkey->SkillIcon->SetColorAndOpacity(FLinearColor::White);
		}

		Hotkey->CooldownMat->SetScalarParameterValue("Percent", 0);
		Hotkey->CooldownImage->SetVisibility(ESlateVisibility::Hidden);
		Hotkey->CooldownText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABaseSkill::PlaySkillAnimation(UAnimMontage* SkillAnimation)
{
	PlayerReference->GetCharacterMovement()->DisableMovement();
	PlayerReference->GetCharacterMovement()->StopMovementImmediately();
	AnimInstance->Montage_Play(SkillAnimation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetMove,this,&ABaseSkill::ResetMovement, SkillAnimation->GetPlayLength(),false);
}

void ABaseSkill::ResetMovement()
{
	PlayerReference->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ABaseSkill::OnSkillNotify()
{
	UE_LOG(LogTemp, Warning, TEXT("OnSkillNotify"));
}

// Called every frame
void ABaseSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseSkill::OnTryCastSpell()
{
	if (!bOnCooldown && !bCurrentCasting && !PlayerReference->GetBIsCasting()) {
		InitializeSpellCast();
	}
}


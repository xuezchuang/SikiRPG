// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/Character/RBaseCharacter.h"
#include "BaseSkill.generated.h"

class ARBaseCharacter;

//基本技能类
UCLASS()
class RPGPROJECT_API ABaseSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSkill();

protected:

	UPROPERTY(EditAnywhere,Category=SkillInfo)
	FSkillInfo SkillInfo;

	int CurrentStageIndex=-1;

	bool bOnCooldown;//是否正在冷却中
	bool bCurrentCasting;//技能释放正在释放

	class USkillHotkey* Hotkey;//与该技能绑定的快捷键

	ARBaseCharacter* PlayerReference;//谁拥有了该技能

	float CurrentCD;

	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere,Category=SkillAnimation)
	UAnimMontage* SkillAniamMontage;

	FTimerHandle TimerHandle_ResetMove;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitializeSpellCast();//初始释放，来判断魔法是否足够

	virtual void OnSpellCast();//技能正在释放的时候，比如播放释放技能的动画

	virtual void OnCastCompleted();//技能结束

	FTimerHandle TimerHandle_Cooldown;

	virtual void OnCooldown();//技能冷却

	void OnCooldownExpired();//技能冷却结束

	virtual void PlaySkillAnimation(UAnimMontage* SkillAnimation);

	void ResetMovement();


	UFUNCTION(BlueprintCallable)
	virtual void OnSkillNotify();



public:	



	// Called every frame
	virtual void Tick(float DeltaTime) override;


	FORCEINLINE FSkillStage GetCurrentStage() { return SkillInfo.Stages[FMath::Clamp(CurrentStageIndex,0,SkillInfo.Stages.Num()-1)]; }
	FORCEINLINE FSkillStage GetNextStage() { return SkillInfo.Stages[FMath::Clamp(CurrentStageIndex+1, 0, SkillInfo.Stages.Num() - 1)]; }
	FORCEINLINE FSkillInfo GetSkillInfo() { return SkillInfo; }

	FORCEINLINE void SetHotkey(class USkillHotkey* HotkeyRef) { this->Hotkey = HotkeyRef; }

	FORCEINLINE bool GetBOnCooldown() { return bOnCooldown; }

	FORCEINLINE ARBaseCharacter* GetPlayerReference() {
		return PlayerReference;
	}

	FORCEINLINE void SetPlayerReference(ARBaseCharacter* PlayerReferenceRef) { this->PlayerReference = PlayerReferenceRef; }

	FORCEINLINE int GetCurrentStageIndex() { return CurrentStageIndex; }

	void ChangeCurrentStageIndex(int DeltaIndex) { CurrentStageIndex=FMath::Clamp(CurrentStageIndex + DeltaIndex,0,SkillInfo.Stages.Num()-1); }

	UFUNCTION(BlueprintCallable)
		virtual void OnTryCastSpell();
	
};

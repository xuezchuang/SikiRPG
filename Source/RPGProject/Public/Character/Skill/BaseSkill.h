// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/Character/RBaseCharacter.h"
#include "BaseSkill.generated.h"

class ARBaseCharacter;

//����������
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

	bool bOnCooldown;//�Ƿ�������ȴ��
	bool bCurrentCasting;//�����ͷ������ͷ�

	class USkillHotkey* Hotkey;//��ü��ܰ󶨵Ŀ�ݼ�

	ARBaseCharacter* PlayerReference;//˭ӵ���˸ü���

	float CurrentCD;

	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere,Category=SkillAnimation)
	UAnimMontage* SkillAniamMontage;

	FTimerHandle TimerHandle_ResetMove;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitializeSpellCast();//��ʼ�ͷţ����ж�ħ���Ƿ��㹻

	virtual void OnSpellCast();//���������ͷŵ�ʱ�򣬱��粥���ͷż��ܵĶ���

	virtual void OnCastCompleted();//���ܽ���

	FTimerHandle TimerHandle_Cooldown;

	virtual void OnCooldown();//������ȴ

	void OnCooldownExpired();//������ȴ����

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

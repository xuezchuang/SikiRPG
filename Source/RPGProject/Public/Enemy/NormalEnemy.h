// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public/Character/Skill/SkillEnum.h"
#include "Public/Interface/DamageableInterface.h"
#include "Public/Interface/SelectableInterface.h"
#include "NormalEnemy.generated.h"

UCLASS()
class RPGPROJECT_API ANormalEnemy : public ACharacter,public IDamageableInterface,public ISelectableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANormalEnemy();

protected:

	float BaseDamage = 25.0f;
	EDamageType DamageType=EDamageType::Physical;
	UPROPERTY(EditAnywhere,Category= Info)
	TSubclassOf<class ABaseElement> Element;
	int CritChance = 25;

	UPROPERTY(EditAnywhere, Category = Info)
	bool bAggressive;//是否是有侵略性的

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category=Behavior)
	bool bDead;

	class ANormalEnemyController* MyController;

	UPROPERTY(VisibleAnywhere,Category=AI)
	class UAIPerceptionComponent* AIPerceptionComp;

	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere,Category=UI)
	class UWidgetComponent* EnemyWidgetComp;
	UPROPERTY(VisibleAnywhere, Category = UI)
	class USphereComponent* ShowUICollision;

	class UEnemyInfoWidget* EnemyInfoWidget;

	UPROPERTY(EditAnywhere, Category = Info)
	FText Name = FText::FromString("Spider");
	UPROPERTY(EditAnywhere, Category = Info)
	int Level = 1;
	UPROPERTY(EditAnywhere, Category = Info)
	float ExpForKill=500.0f;
	UPROPERTY(EditAnywhere, Category = Info)
	bool bDoesRespawn = true;
	UPROPERTY(EditAnywhere, Category = Info)
	float RespawnTime=10;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSightPerceptionUpdate(const TArray<AActor*>& UpdatedActors);

	UFUNCTION(BlueprintCallable)
	void NotifyHit();

	void AttackRay();

	bool bInShowRange;

	//当主角里敌人很近的时候显示信息，很近：与球形碰撞体发生碰撞的时候
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void InitWidgetText();

	virtual void OnReceiveDamage(float BaseDamageRef, EDamageType Type, TSubclassOf<ABaseElement> Element, int CritChanceRef, AActor* Attacker, ABaseSkill* Spell) ;

	void OnDeath(AActor* Killer);

	void Respawn();

	FTimerHandle TimerHandle_Respawn;

	bool bSelected = false;

	class ARBaseCharacter* SelectingCharacter;



	virtual void OnSelected(class ARBaseCharacter* Character) override;//选中的时候
	virtual void OnSelectionEnd(class ARBaseCharacter* Character)override;//选择之后离开

	FTimerHandle TimerHandle_DelayDeath;
	void OnDelayDeath();
public:	
	FVector StartLocation;//初始的位置
	UPROPERTY(VisibleAnywhere, Category = Hit)
		class UArrowComponent* HitArrow;
	UPROPERTY(EditAnywhere, Category = Info)
		float TotalHealth = 50.0f;

	float CurrentHealth;

	UPROPERTY(EditAnywhere,Category=Behavior)
	TArray<class UAnimMontage*> AttackAnimaions;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateHealthBar();
	
	FORCEINLINE bool GetBDead() { return bDead; }
};

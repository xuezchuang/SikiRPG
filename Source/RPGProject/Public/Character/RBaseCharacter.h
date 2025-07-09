// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CursorDecal.h"
#include "Public/Character/Skill/SkillEnum.h"
#include "Public/Interface/DamageableInterface.h"
#include "Public/Quest/QuestEnum.h"
#include "RBaseCharacter.generated.h"

class ABaseSkill;
class UTexture2D;
class ABaseElement;
class UBuffWidget;
class ABuffSkill;

//技能的阶段（等级），每个阶段属性值是不同的
USTRUCT()
struct FSkillStage {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,Category=SkillData)
	int RequiredLevel;//需要前置的等级
	UPROPERTY(EditAnywhere, Category = SkillData)
	TArray<TSubclassOf<ABaseSkill>> RequiredSkills;//需要的前置技能
	UPROPERTY(EditAnywhere, Category = SkillData)
	UTexture2D* OverrideIcon;//覆盖的图标，如果为空，就是用初始技能的图标
	UPROPERTY(EditAnywhere, Category = SkillData)
	float Range;//技能效果的范围
	UPROPERTY(EditAnywhere, Category = SkillData)
	float Damage;//技能的伤害值
	UPROPERTY(EditAnywhere, Category = SkillData)
	EDamageType DamageType;//伤害类型
	UPROPERTY(EditAnywhere, Category = SkillData)
	int CriticalChance;//暴击率
	UPROPERTY(EditAnywhere, Category = SkillData)
	float CooldownTime;//冷却时间
	UPROPERTY(EditAnywhere, Category = SkillData)
	float CastingTime;//释放的时间
	UPROPERTY(EditAnywhere, Category = SkillData)
	float ManaCost;//魔法消耗值
	UPROPERTY(EditAnywhere, Category = SkillData)
	float MisiileSpeed;//投掷类技能的速度，比如 弓箭速度

};

//技能的信息
USTRUCT()
struct FSkillInfo{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, Category = SkillData)
	FText Name;
	UPROPERTY(EditAnywhere, Category = SkillData)
	UTexture2D* Icon;
	UPROPERTY(EditAnywhere, Category = SkillData)
	FText Description;//关于技能的描述
	UPROPERTY(EditAnywhere, Category = SkillData)
	TSubclassOf<ABaseElement> Element;//该技能属于哪一个元素
	UPROPERTY(EditAnywhere, Category = SkillData)
	ETargetTypes Target;//作用对象
	UPROPERTY(EditAnywhere, Category = SkillData)
	TArray<FSkillStage> Stages;//技能所拥有的所有阶段，通过SkillInfo来获取SkillStage
};

UCLASS()
class RPGPROJECT_API ARBaseCharacter : public ACharacter,public IDamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARBaseCharacter();
	int TestInt = 9;
protected:


	UPROPERTY(VisibleAnywhere,Category="Camera")
	class UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere,Category="Item")
	class UParticleSystemComponent* OverloadParticleComp;

	UPROPERTY(EditAnywhere,Category=Mouse)
	TSubclassOf<class ACursorDecal> CursorDecal;

	bool bHasMouseMoveCommand = false;

	class ACursorDecal* CurrentCursor;

	UPROPERTY(EditAnywhere,Category=Camera)
	float MinCameraZoom_V=75.0f;
	UPROPERTY(EditAnywhere, Category = Camera)
	float MaxCameraZoom_V=1000.0f;
	UPROPERTY(EditAnywhere, Category = Camera)
	float CameraZoomAlpha=25.0f;

	class AMiniCam* MiniCam;
	UPROPERTY(VisibleAnywhere,Category=Camera)
	class USceneCaptureComponent2D* PortraitComponent;


	UPROPERTY(EditAnywhere,Category=Skill)
	TSubclassOf<ABaseElement> Element;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void OnSetDestinationPressed();

	void SetNewMoveDestination(const FVector DesLocaiton);

	void MoveToCursor();

	void CancleMoveToCursor();

	void CameraZoomIn();

	void CameraZoomOut();

	void ToogleShowSkillTree();

	void ToogleShowQuest();

	void ToogleShowInventory();

	void InteractToNPC();

	void TestCompleteQuest();
	void TestFailQuest();

	void OnShiftPressed();
	void OnShiftReleased();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	virtual void OnReceiveDamage(float BaseDamage, EDamageType Type, TSubclassOf<ABaseElement> ElementRef, int CritChance, AActor* Attacker, ABaseSkill* Spell)  override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "PlayerInfo")
	FString CharacterName;
	UPROPERTY(VisibleAnywhere,Category="PlayerInfo")
	float TotalHp=100;
	UPROPERTY(VisibleAnywhere, Category = "PlayerInfo")
	float TotalMp=100;
	UPROPERTY(VisibleAnywhere, Category = "PlayerInfo")
	float CurrentHp;
	UPROPERTY(VisibleAnywhere, Category = "PlayerInfo")
	float CurrentMp;
	UPROPERTY(VisibleAnywhere, Category = "PlayerInfo")
	float CurrentExp;
	UPROPERTY(VisibleAnywhere, Category = "PlayerInfo")
	int CurrentLevel;

	void ReadData();

protected:
	UPROPERTY(EditAnywhere,Category=UI)
	TArray<FKey> Keys;
	UPROPERTY(EditAnywhere, Category = UI)
	int KeysPerRow;

	bool bCanFindKey = true;

	float DefaultSpeed;

	//UPROPERTY(EditAnywhere,Category=Skill)
	//TArray<TSubclassOf<ABaseSkill>> StartingSkills;//初始技能


	UPROPERTY(BlueprintReadOnly)
	ABaseSkill* CurrentSkill;//当前技能

	//void GenerateStartingSkills();//生成初始技能

	void OnAnyKeyPressed(FKey Key);

	bool bIsCasting;

	AActor* SelectedActor;//之前被点击的Actor

	UPROPERTY(EditAnywhere, Category = Particle)
		class UParticleSystem* LevelupParticle;

	void CancleMissile();

	void IncreaseLevel();

	UPROPERTY(VisibleAnywhere,Category=Data)
	float NeededExpToNextLevel;

	UPROPERTY(VisibleAnywhere)
	class USkillTreeComponent* SkillTreeComp;

public:	

	class ANormalEnemy* SelectedEnemy;

	class AMissileSkill* MissileSkill;

	TArray<TSubclassOf<ABaseSkill>> LearntSkills;//主角已经学习过了的技能

	class  APlayerController* PC;
	UPROPERTY(BlueprintReadOnly)
		class UMainUserWidget* MainUserWidget;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void BeginSpellCast(ABaseSkill* Skill); //开始技能释放

	void EndSpellCast(ABaseSkill* Skill);//结束技能释放
	void ChangeCurrentHp(float DeltaHp);
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	void ChangeCurrentMp(float DeltaMp);
	FORCEINLINE float GetCurrentMp() { return CurrentMp; }
	FORCEINLINE float GetTotalHp() { return TotalHp; }
	FORCEINLINE float GetTotalMp() { return TotalMp; }
	void IncreaseCurrentExp(float DeltaExp);
	FORCEINLINE void SetCharacterName(FString Name) { CharacterName = Name; }
	FORCEINLINE FString GetCharacterName() { return CharacterName; }
	FORCEINLINE bool GetBIsCasting() { return bIsCasting; }
	FORCEINLINE int GetCurrentLevel() { UE_LOG(LogTemp, Warning, TEXT("CurrentLevel:%s"), *FString::SanitizeFloat(CurrentLevel)); return CurrentLevel; }

public:
	TArray< ABuffSkill*> Buffs;

	UBuffWidget* BuffWidget;

	 UBuffWidget* AddBuff(ABuffSkill* SkillBuff);
	 void RemoveBuff(ABuffSkill* SkillBuff);

protected:
	UPROPERTY(EditAnywhere,Category=Quest)
	TSubclassOf<class AQuestManager> QuestManagetClass;


	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* InteractionComp;
	UFUNCTION()
		void OnInteractionCompBeginOverlap(UPrimitiveComponent*  OverlappedComponent, AActor*  OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void OnIteractionCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:

	UPROPERTY(BlueprintReadOnly)
	class AQuestManager* QuestManager;

	UFUNCTION(BlueprintImplementableEvent)
		void PlaySlideOutAnim();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAddNewQuestAnim();
public:
	TArray<TSubclassOf<class ABaseQuestProp>> ObtainedProps;

protected:
	UPROPERTY(EditAnywhere, Category = Inventory)
		TSubclassOf<class AInventory> InventoryClass;
public:
	UPROPERTY(BlueprintReadOnly)
	class AInventory* InventoryRef;

	bool bShiftDown;

	class AItemStaff* Staff;

	bool EquipItem(class AItemStaff* ItemStaff);

	UFUNCTION(BlueprintCallable)
	bool UnEquipItem();

	void OnOverloaded();

	void OnOverloadEnd();

	void Test(TArray<int> &IntTest);

public:
	int CurrentCoin = 0;
	void IncreaseCoin(int Amount);
	void DecreaseCoin(int Amount);

public:
	class URPGSave* RPGSaveInstance;
	FString SaveSlotName = "RPGSave";

	void SaveGame();
	void LoadGame();
protected:

	UPROPERTY(EditAnywhere, Category = ItemInfo)
		TArray<FKey> ItemKeys;
};

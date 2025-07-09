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

//���ܵĽ׶Σ��ȼ�����ÿ���׶�����ֵ�ǲ�ͬ��
USTRUCT()
struct FSkillStage {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,Category=SkillData)
	int RequiredLevel;//��Ҫǰ�õĵȼ�
	UPROPERTY(EditAnywhere, Category = SkillData)
	TArray<TSubclassOf<ABaseSkill>> RequiredSkills;//��Ҫ��ǰ�ü���
	UPROPERTY(EditAnywhere, Category = SkillData)
	UTexture2D* OverrideIcon;//���ǵ�ͼ�꣬���Ϊ�գ������ó�ʼ���ܵ�ͼ��
	UPROPERTY(EditAnywhere, Category = SkillData)
	float Range;//����Ч���ķ�Χ
	UPROPERTY(EditAnywhere, Category = SkillData)
	float Damage;//���ܵ��˺�ֵ
	UPROPERTY(EditAnywhere, Category = SkillData)
	EDamageType DamageType;//�˺�����
	UPROPERTY(EditAnywhere, Category = SkillData)
	int CriticalChance;//������
	UPROPERTY(EditAnywhere, Category = SkillData)
	float CooldownTime;//��ȴʱ��
	UPROPERTY(EditAnywhere, Category = SkillData)
	float CastingTime;//�ͷŵ�ʱ��
	UPROPERTY(EditAnywhere, Category = SkillData)
	float ManaCost;//ħ������ֵ
	UPROPERTY(EditAnywhere, Category = SkillData)
	float MisiileSpeed;//Ͷ���༼�ܵ��ٶȣ����� �����ٶ�

};

//���ܵ���Ϣ
USTRUCT()
struct FSkillInfo{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, Category = SkillData)
	FText Name;
	UPROPERTY(EditAnywhere, Category = SkillData)
	UTexture2D* Icon;
	UPROPERTY(EditAnywhere, Category = SkillData)
	FText Description;//���ڼ��ܵ�����
	UPROPERTY(EditAnywhere, Category = SkillData)
	TSubclassOf<ABaseElement> Element;//�ü���������һ��Ԫ��
	UPROPERTY(EditAnywhere, Category = SkillData)
	ETargetTypes Target;//���ö���
	UPROPERTY(EditAnywhere, Category = SkillData)
	TArray<FSkillStage> Stages;//������ӵ�е����н׶Σ�ͨ��SkillInfo����ȡSkillStage
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
	//TArray<TSubclassOf<ABaseSkill>> StartingSkills;//��ʼ����


	UPROPERTY(BlueprintReadOnly)
	ABaseSkill* CurrentSkill;//��ǰ����

	//void GenerateStartingSkills();//���ɳ�ʼ����

	void OnAnyKeyPressed(FKey Key);

	bool bIsCasting;

	AActor* SelectedActor;//֮ǰ�������Actor

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

	TArray<TSubclassOf<ABaseSkill>> LearntSkills;//�����Ѿ�ѧϰ���˵ļ���

	class  APlayerController* PC;
	UPROPERTY(BlueprintReadOnly)
		class UMainUserWidget* MainUserWidget;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void BeginSpellCast(ABaseSkill* Skill); //��ʼ�����ͷ�

	void EndSpellCast(ABaseSkill* Skill);//���������ͷ�
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

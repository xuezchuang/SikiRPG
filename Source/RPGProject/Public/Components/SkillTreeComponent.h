// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/Character/Skill/SkillTreeStruct.h"
#include "SkillTreeComponent.generated.h"

class ABaseSkill;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API USkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	class ARBaseCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere,Category=Skill)
	int SkillPoints = 2;//技能点

	UPROPERTY(EditAnywhere, Category = Skill)
	TArray<FTreeCategory> Categories;

	// Sets default values for this component's properties
	USkillTreeComponent();

protected:


	bool bTreeSetup;//技能树里面的内容是否已经设置好

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	class UMainTreeWidget* MainTreeWidget;
	ABaseSkill* UpgradedSpell;
	class USkillTreeEntryWidget* EntryWidget;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool BPlayerLearnedSpell(TSubclassOf<ABaseSkill> Skill);

	void AddSkillPoints(int Amount);

	bool BCanUpgradeSpell(ABaseSkill* Spell);

	bool BSkillOnHotkey(ABaseSkill* Spell);

	void UpgradeSpell(ABaseSkill* SkillActor,class USkillTreeEntryWidget* EntryWidgetRef);
	void DowngradeSpell(ABaseSkill* SkillActor, class USkillTreeEntryWidget* EntryWidgetRef);
	
	void HandleShowCommand();
	void SetupTree();

	void ActiveConnections(TSubclassOf<ABaseSkill> ForSpell);

	void UpdateAllEntries();
};

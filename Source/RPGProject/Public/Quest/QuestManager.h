// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/Quest/QuestStruct.h"
#include "Public//Enemy/NormalEnemy.h"
#include "QuestManager.generated.h"

UCLASS()
class RPGPROJECT_API AQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestManager();

	class ARBaseCharacter* PlayerCharacter;

	class UMainUserWidget* MainUserWidget;

	class ABaseQuest* CurrentQuest;//当前处理的任务

	TArray<TSubclassOf<class ABaseQuest>> AllQuestClasses;
	TArray<class ABaseQuest*> CurrentQuestActors;
	TArray<class ABaseQuest*> CompletedQuestActors;
	TArray<class ABaseQuest*> FailedQuestActors;

	FGoalInfo CurrentSubGoal;

	class AGoalActor* CurrentGoal;

	UPROPERTY(EditAnywhere,Category=Quest)
	TSubclassOf<class AGoalActor> GoalActorClass;

	float GetDistanceToGoal();

	void UpdateDirectionArrow();

	void OnSwitchSubQuest();

	void OnPlayerMove();

protected:

	UPROPERTY(EditAnywhere,Category=Quest)
	float ShowHintDistance;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SelectNewQuest(class ABaseQuest* NewQuest, class USubGoalWidget* SubGoalWidget);

	UFUNCTION(BlueprintCallable)
		bool AddNewQuest(TSubclassOf<class ABaseQuest> QuestClass);

	void OnEnemyKilled(TSubclassOf<class ANormalEnemy> Enemy);
	void OnObjectFound(TSubclassOf<class ABaseQuestProp> Prop);
	void OnTalkToNPC(TSubclassOf<class ABaseNPC> NPC, int NPCId);

	void OnQuestEnd(class ABaseQuest* Quest);
	bool bCurrentQuestFinished;
};

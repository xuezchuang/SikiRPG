// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public/Interface/InterationInterface.h"
#include "BaseNPC.generated.h"

UCLASS()
class RPGPROJECT_API ABaseNPC : public ACharacter, public IInterationInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* InteractionWidget;
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* MessageWidgetComp;

	UPROPERTY(VisibleAnywhere)
	class UPaperSpriteComponent* QuestIcon;

	UPROPERTY(EditAnywhere, Category = Quest)
	FText Name;
	UPROPERTY(EditAnywhere, Category = Quest)
	TSubclassOf<class ABaseQuest> MyQuest;

	UPROPERTY(EditAnywhere, Category = Quest)
	class UMessageWidget* MessageWidget;

	UPROPERTY(VisibleAnywhere, Category = Quest)
	bool bInPlayerRadius;

	FTimerHandle TimerHandle_ResetMessage;

	void ResetMessage();
public:
	UPROPERTY(EditAnywhere, Category = Quest)
	bool bHasQuest;

	UPROPERTY(EditAnywhere, Category = Quest)
	int NPCId;
	UPROPERTY(EditAnywhere, Category = Quest)
	bool bCanTalkTo;
	UPROPERTY(EditAnywhere, Category = Quest)
	FText Message;
	UPROPERTY(EditAnywhere, Category = Quest)
	float DefaultDuration = 5.0f;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnEnterPlayerRadius(class ARBaseCharacter* Character) override;
	virtual void OnLeavePlayerRadius(class ARBaseCharacter* Character) override;
	virtual void OnIteractWith(class ARBaseCharacter* Character) override;


	void OnTalkWith(class ARBaseCharacter* Character);

	void ShowMessage(FText MessageRef, float Duration, class ARBaseCharacter* Character);
};

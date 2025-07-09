// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/Interface/InterationInterface.h"
#include "BaseQuestProp.generated.h"

UCLASS()
class RPGPROJECT_API ABaseQuestProp : public AActor,public IInterationInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseQuestProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* InteractionWidget;
	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ParticleSystemComp;

	UPROPERTY(EditAnywhere,Category=Info)
	FText Name;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
public:
	virtual void OnEnterPlayerRadius(class ARBaseCharacter* Character) override;


	virtual void OnLeavePlayerRadius(class ARBaseCharacter* Character) override;


	virtual void OnIteractWith(class ARBaseCharacter* Character) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/Interface/InterationInterface.h"
#include "Public/InveotorySystem/InventoryStruct.h"
#include "BaseItem.generated.h"

UCLASS()
class RPGPROJECT_API ABaseItem : public AActor,public IInterationInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:

	

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* InterationWidgetComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollisonComp;

	UPROPERTY(EditAnywhere, Category = ItemInfo)
		FItemInfo ItemInfo;
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	int Amount;
	
	int Index;
	UPROPERTY(EditAnywhere, Category = ItemInfo)
	int ID=-1;

	class AInventory* InventoryRef;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnEnterPlayerRadius(class ARBaseCharacter* Character) override;
	virtual void OnLeavePlayerRadius(class ARBaseCharacter* Character) override;
	virtual void OnIteractWith(class ARBaseCharacter* Character) override;
	
	virtual void OnUsed();
};

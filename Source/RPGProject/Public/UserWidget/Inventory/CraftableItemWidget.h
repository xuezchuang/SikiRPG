// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftableItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UCraftableItemWidget : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* NameText;
	class UCraftingMenuWidget* CraftingMenu;

	UFUNCTION()
		void OnSelectButtonClicked();

	virtual void NativeConstruct() override;

public:
	TSubclassOf<class ABaseItem> Item;
	UPROPERTY(Meta = (BindWidget))
		class UButton* SelectButton;
	void Init(TSubclassOf<class ABaseItem> ItemRef,class UCraftingMenuWidget* CraftingMenuRef);
};

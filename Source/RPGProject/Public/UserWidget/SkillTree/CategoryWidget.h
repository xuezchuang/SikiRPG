// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CategoryWidget.generated.h"

/**
 * 技能树的种类，包括技能，防御，Buff等
 */
UCLASS()
class RPGPROJECT_API UCategoryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta=(BindWidget))
	class UButton* CategoryButton;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* CategoryNameText;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnCategoryButtonHoverd();

	UFUNCTION()
		void OnCategoryButtonUnHoverd();

	UFUNCTION()
		void OnCategoryButtonClicked();
public:
	void ChangeState(bool bSelectedRef);

	FText CategoryName;

	bool bSelected;//当前的这个Category是否被选择，点击
	int Index;//当前Category所对应的索引

	class UMainTreeWidget* MainTreeRef;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CategoryWidget.generated.h"

/**
 * �����������࣬�������ܣ�������Buff��
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

	bool bSelected;//��ǰ�����Category�Ƿ�ѡ�񣬵��
	int Index;//��ǰCategory����Ӧ������

	class UMainTreeWidget* MainTreeRef;

};

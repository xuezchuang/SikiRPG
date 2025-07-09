// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadableItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UReadableItemWidget : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(Meta=(BindWidget))
		class UTextBlock* NameText;
	UPROPERTY(Meta = (BindWidget))
	class UButton* CloseButton;
	UPROPERTY(Meta = (BindWidget))
	class UImage* MapImage;

	UFUNCTION()
		void OnCloseButtonClicked();

	virtual void NativeConstruct() override;
	
public:
	class AItemMap* ItemMap;
};

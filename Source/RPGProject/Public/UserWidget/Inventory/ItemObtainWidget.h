// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemObtainWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UItemObtainWidget : public UUserWidget
{
	GENERATED_BODY()

		UPROPERTY(Meta=(BindWidget))
		UTextBlock* NameText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* AmountText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ObtainText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* XText;
	UPROPERTY(Meta = (BindWidget))
	class UImage* IconImage;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float VisibleDuration=2.0f;

	UPROPERTY(BlueprintReadOnly)
	class UMainUserWidget* MainUserWidget;
public:
	void Init(TSubclassOf<class ABaseItem> ItemClass,int Amount, class UMainUserWidget* MainUserWidgetRef);
};

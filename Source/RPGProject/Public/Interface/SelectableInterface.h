// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "SelectableInterface.generated.h"

UINTERFACE()
class USelectableInterface :public UInterface {
	GENERATED_BODY()
};

class ISelectableInterface {
	GENERATED_BODY()

public:
	virtual void OnSelected(class ARBaseCharacter* Character) = 0;//ѡ�е�ʱ��
	virtual void OnSelectionEnd(class ARBaseCharacter* Character) = 0;//ѡ��֮���뿪
};
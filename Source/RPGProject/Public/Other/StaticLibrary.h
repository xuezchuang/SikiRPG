// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StaticLibrary.generated.h"

class ABaseElement;

/**
 *
 */
UCLASS()
class RPGPROJECT_API UStaticLibrary : public UObject
{
	GENERATED_BODY()

public:
	static float CaculateFinalDamage(float BaseDamage, int CritChance, TSubclassOf<ABaseElement> AttackerElement, TSubclassOf<ABaseElement> DefenderElement);
	static bool BIsEnemy(AActor* Actor);

	template<typename TEnum>
	static FString GetEnumValueAsString(const FString& Name, TEnum Value);

	template<typename TEnum>
	static TArray<TEnum> EnumGetList();
};

template<typename TEnum>
FString UStaticLibrary::GetEnumValueAsString(const FString& Name, TEnum Value)
{
	//const UEnum* EnumPtr= FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	const UEnum* EnumPtr = FindObject<UEnum>(nullptr, *Name);
	if (!EnumPtr)
	{
		return FString("InValid");
	}
	else
	{
		FString TempString = EnumPtr->GetNameByValue((int64)Value).ToString();
		TempString.RemoveFromStart(Name + "::");
		return TempString;
	}
}

template<typename TEnum>
TArray<TEnum> UStaticLibrary::EnumGetList()
{
	TArray<TEnum> Result;
	const UEnum* pEnum = StaticEnum<TEnum>();
	if (!pEnum)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnumGetList: StaticEnum returned nullptr"));
		return Result;
	}

	for (int32 i = 0; i < pEnum->NumEnums(); ++i)
	{
		// 防止包含 _MAX 结尾的无效项
		if (!pEnum->HasMetaData(TEXT("Hidden"), i))
		{
			Result.Add(static_cast<TEnum>(pEnum->GetValueByIndex(i)));
		}
	}
	return Result;
}

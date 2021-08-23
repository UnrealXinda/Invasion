// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InvasionMathHelper.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API UInvasionMathHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()	

public:

	/** Check to see if a given angle is inside the range of the smaller angle formed by two angles */
	UFUNCTION(BlueprintPure, Category = Miscellaneous)
	static bool IsAngleInsideRange(float Angle, float Min, float Max);
};

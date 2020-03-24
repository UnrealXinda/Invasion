// Fill out your copyright notice in the Description page of Project Settings.


#include "InvasionMathHelper.h"

bool UInvasionMathHelper::IsAngleInsideRange(float Angle, float Min, float Max)
{
	FVector MinVec = FRotator(0.0f, Min, 0.0f).Vector();
	FVector MaxVec = FRotator(0.0f, Max, 0.0f).Vector();
	FVector CurrentVec = FRotator(0.0f, Angle, 0.0f).Vector();

	bool bOnSameSideOfMin = ((MinVec ^ CurrentVec).Z * (MinVec ^ MaxVec).Z > 0);
	bool bOnSameSideOfMax = ((MaxVec ^ CurrentVec).Z * (MaxVec ^ MinVec).Z > 0);

	return bOnSameSideOfMin && bOnSameSideOfMax;
}

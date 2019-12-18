// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvasionTypes.h"
#include "InvasionEnums.h"
#include "InvasionDebug.h"
#include "InvasionMacros.h"
#include "InvasionDelegates.h"

#define SURFACE_FLESH_DEFAULT      SurfaceType1
#define SURFACE_FLESH_VULNERABLE   SurfaceType2

#define COLLISION_WEAPON           ECC_GameTraceChannel1

namespace InvasionStatics
{
#define DefineInputBindingNames(ActionName) const FName ActionName(#ActionName);

	DefineInputBindingNames(MoveForward);
	DefineInputBindingNames(MoveRight);
	DefineInputBindingNames(Turn);
	DefineInputBindingNames(TurnRate);
	DefineInputBindingNames(LookUp);
	DefineInputBindingNames(LookUpRate);

	DefineInputBindingNames(Aim);
	DefineInputBindingNames(Dash);
	DefineInputBindingNames(Fire);
	DefineInputBindingNames(Sprint);
	DefineInputBindingNames(TakeCover);
}
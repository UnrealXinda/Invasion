// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvasionTypes.h"
#include "InvasionEnums.h"
#include "InvasionDebug.h"
#include "InvasionMacros.h"

// Custom Object Channels
#define ECC_CoverObject      ECC_GameTraceChannel2

// Custom Trace Channels
#define ECC_Weapon           ECC_GameTraceChannel1

// Custom collision profiles
#define CollisionProfile_Cover      TEXT("Cover")


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
	DefineInputBindingNames(Scan);
	DefineInputBindingNames(Fire);
	DefineInputBindingNames(Sprint);
	DefineInputBindingNames(TakeCover);
	DefineInputBindingNames(Execute);
}
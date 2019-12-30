// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETimeGroup : uint8
{
	World  = 0   UMETA(DisplayName = "World"),
	Enemy  = 1   UMETA(DisplayName = "Enemy"),
	Player = 2   UMETA(DisplayName = "Player"),
	UI     = 3   UMETA(DisplayName = "UI"),
	System = 4   UMETA(DisplayName = "System"),
	Max    = 5,
};

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Walk   = 0   UMETA(DisplayName = "Walk"),
	Run    = 1   UMETA(DisplayName = "Run"),
	Sprint = 2   UMETA(DisplayName = "Sprint"),
	Max    = 3,
};

UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	Left     = 0   UMETA(DisplayName = "Left"),
	Right    = 1   UMETA(DisplayName = "Right"),
	Max      = 2,
};

UENUM(BlueprintType)
enum class EAimState : uint8
{
	Idle   = 0   UMETA(DisplayName = "Idle"),
	Aiming = 1   UMETA(DisplayName = "Aimimg"),
	Max    = 2,
};

UENUM(BlueprintType)
enum class EExecuteState : uint8
{
	Idle      = 0   UMETA(DisplayName = "Idle"),
	Executing = 1   UMETA(DisplayName = "Executing"),
	Max       = 2,
};

UENUM(BlueprintType)
enum class ECoverState : uint8
{
	Idle    = 0   UMETA(DisplayName = "Idle"),
	HighIn  = 1   UMETA(DisplayName = "HighIn"),
	HighOut = 2   UMETA(DisplayName = "HighOut"),
	LowIn   = 3   UMETA(DisplayName = "LowIn"),
	LowOut  = 4   UMETA(DisplayName = "LowOut"),
	InCover = 5   UMETA(DisplayName = "InCover"),
	Max     = 6,
};

UENUM(BlueprintType)
enum class EDashState : uint8
{
	Idle    = 0   UMETA(DisplayName = "Idle"),
	Dashing = 1   UMETA(DisplayName = "Dashing"),
	Max     = 2,
};

UENUM(BlueprintType)
enum class ECoverType : uint8
{
	Low  = 0  UMETA(DisplayName = "Low"),
	High = 1  UMETA(DisplayName = "High"),
	Max  = 2,
};
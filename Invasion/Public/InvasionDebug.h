// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"

namespace InvasionDebug
{
	extern int g_DebugDrawWeaponTrace;
	extern FAutoConsoleVariableRef CVarDebugDrawWeaponTrace;

	extern int g_DebugDrawIKTrace;
	extern FAutoConsoleVariableRef CVarDebugDrawIKTrace;

	extern int g_DebugDrawAISightTrace;
	extern FAutoConsoleVariableRef CVarDebugDrawAISightTrace;

	extern int g_PlayerGodMode;
	extern FAutoConsoleVariableRef CVarPlayerGodMode;

	extern int g_PlayerInfiniteEnergy;
	extern FAutoConsoleVariableRef CVarPlayerInfiniteEnergy;
}
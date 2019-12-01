// Fill out your copyright notice in the Description page of Project Settings.

#include "InvasionDebug.h"
#include "Modules/ModuleManager.h"

int32 InvasionDebug::g_DebugDrawWeaponTrace = 0;

FAutoConsoleVariableRef InvasionDebug::CVarDebugDrawWeaponTrace(
	TEXT("Invasion.DebugDrawWeaponTrace"),
	InvasionDebug::g_DebugDrawWeaponTrace,
	TEXT("Draw debug weapon trace"),
	ECVF_Cheat);
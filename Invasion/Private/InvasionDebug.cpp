// Fill out your copyright notice in the Description page of Project Settings.

#include "InvasionDebug.h"
#include "Modules/ModuleManager.h"

int32 InvasionDebug::g_DebugDrawWeaponTrace = 0;
int32 InvasionDebug::g_DebugDrawIKTrace = 0;
int32 InvasionDebug::g_DebugDrawAISightTrace = 0;

FAutoConsoleVariableRef InvasionDebug::CVarDebugDrawWeaponTrace(
	TEXT("Invasion.DebugDrawWeaponTrace"),
	InvasionDebug::g_DebugDrawWeaponTrace,
	TEXT("Draw debug weapon trace"),
	ECVF_Cheat);

FAutoConsoleVariableRef InvasionDebug::CVarDebugDrawIKTrace(
	TEXT("Invasion.DebugDrawIKTrace"),
	InvasionDebug::g_DebugDrawIKTrace,
	TEXT("Draw debug IK trace"),
	ECVF_Cheat);

FAutoConsoleVariableRef InvasionDebug::CVarDebugDrawAISightTrace(
	TEXT("Invasion.DebugDrawAISightTrace"),
	InvasionDebug::g_DebugDrawAISightTrace,
	TEXT("Draw debug AI Sight Trace"),
	ECVF_Cheat);
// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InvasionPlayerConfiguration.h"

UInvasionPlayerConfiguration::UInvasionPlayerConfiguration()
{
	InputSetting.BaseLookUpRate = 45.f;
	InputSetting.BaseTurnRate = 45.f;
	InputSetting.WalkInputThreshold = 0.8F;
}

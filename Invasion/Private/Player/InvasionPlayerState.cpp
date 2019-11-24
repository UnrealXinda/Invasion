// Fill out your copyright notice in the Description page of Project Settings.

#include "InvasionPlayerState.h"
#include "InvasionPlayerConfiguration.h"

AInvasionPlayerState::AInvasionPlayerState()
{
	static ConstructorHelpers::FClassFinder<UInvasionPlayerConfiguration> DefaultPlayerConfig(TEXT("/Game/Configurations/BP_PlayerConfiguration"));
	PlayerConfigurationClass = DefaultPlayerConfig.Class;

	UWorld* World = GetWorld();

	if (World && PlayerConfigurationClass)
	{
		PlayerConfiguration = NewObject<UInvasionPlayerConfiguration>(World, PlayerConfigurationClass);
	}
}

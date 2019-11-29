// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/InvasionGameMode.h"

#include "Player/InvasionPlayerController.h"
#include "Player/InvasionPlayerState.h"

#include "Systems/TimeDilationSystem.h"

AInvasionGameMode::AInvasionGameMode()
{
	PlayerControllerClass = AInvasionPlayerController::StaticClass();
	PlayerStateClass = AInvasionPlayerState::StaticClass();

	TimeDilationSystemClass = ATimeDilationSystem::StaticClass();
}

ATimeDilationSystem* AInvasionGameMode::GetTimeDilationSystem() const
{
	return TimeDilationSystem;
}

void AInvasionGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TimeDilationSystem = GetWorld()->SpawnActor<ATimeDilationSystem>(TimeDilationSystemClass, Params);

	check(TimeDilationSystem);
}

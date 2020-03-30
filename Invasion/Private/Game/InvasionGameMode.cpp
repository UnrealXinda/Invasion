// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/InvasionGameMode.h"

#include "Player/InvasionPlayerController.h"
#include "Player/InvasionPlayerState.h"

#include "Systems/TimeDilationSystem.h"
#include "Systems/PostProcessSystem.h"
#include "Systems/CoverSystem.h"

#include "Kismet/KismetMathLibrary.h"

AInvasionGameMode::AInvasionGameMode()
{
	PlayerControllerClass = AInvasionPlayerController::StaticClass();
	PlayerStateClass = AInvasionPlayerState::StaticClass();

	TimeDilationSystemClass = ATimeDilationSystem::StaticClass();
	CoverSystemClass = ACoverSystem::StaticClass();
	PostProcessSystemClass = APostProcessSystem::StaticClass();

	MaximumExecutionDistance = 150.0f;
}

ATimeDilationSystem* AInvasionGameMode::GetTimeDilationSystem() const
{
	return TimeDilationSystem;
}

APostProcessSystem* AInvasionGameMode::GetPostProcessSystem() const
{
	return PostProcessSystem;
}

ACoverSystem* AInvasionGameMode::GetCoverSystem() const
{
	return CoverSystem;
}

void AInvasionGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TimeDilationSystem = GetWorld()->SpawnActor<ATimeDilationSystem>(TimeDilationSystemClass, Params);
	CoverSystem = GetWorld()->SpawnActor<ACoverSystem>(CoverSystemClass, Params);
	PostProcessSystem = GetWorld()->SpawnActor<APostProcessSystem>(PostProcessSystemClass, Params);

	check(TimeDilationSystem);
	check(CoverSystem);
	check(PostProcessSystem);

	check(ExecutionData);
}

const FExecutionScript& AInvasionGameMode::GetRandomExecutionScript() const
{
	int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, ExecutionData->ExecutionScripts.Num() - 1);
	return ExecutionData->ExecutionScripts[Index];
}

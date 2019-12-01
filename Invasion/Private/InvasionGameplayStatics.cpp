// Fill out your copyright notice in the Description page of Project Settings.


#include "InvasionGameplayStatics.h"

#include "Game/InvasionGameMode.h"
#include "Game/InvasionGameState.h"

#include "Player/InvasionPlayerState.h"
#include "Player/InvasionPlayerController.h"
#include "Player/InvasionPlayerCameraManager.h"

#include "Systems/TimeDilationSystem.h"

#include "Characters/InvasionCharacter.h"
#include "Characters/InvasionPlayerCharacter.h"

#include "Kismet/GameplayStatics.h"

bool UInvasionGameplayStatics::IsInEditor()
{
	return GIsEditor;
}

AInvasionGameMode* UInvasionGameplayStatics::GetInvasionGameMode(const UObject* WorldContextObject)
{
	return Cast<AInvasionGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
}

AInvasionGameState* UInvasionGameplayStatics::GetInvasionGameState(const UObject* WorldContextObject)
{
	return Cast<AInvasionGameState>(UGameplayStatics::GetGameState(WorldContextObject));
}

AInvasionPlayerController* UInvasionGameplayStatics::GetInvasionPlayerController(const UObject* WorldContextObject)
{
	return Cast<AInvasionPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
}

AInvasionPlayerCharacter* UInvasionGameplayStatics::GetInvasionPlayerCharacter(const UObject* WorldContextObject)
{
	return Cast<AInvasionPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));
}

class AInvasionPlayerCameraManager* UInvasionGameplayStatics::GetInvasionPlayerCameraManager(const UObject* WorldContextObject)
{
	AInvasionPlayerController* Controller = GetInvasionPlayerController(WorldContextObject);

	return Controller ? Cast<AInvasionPlayerCameraManager>(Controller->PlayerCameraManager) : nullptr;
}

class ATimeDilationSystem* UInvasionGameplayStatics::GetTimeDilationSystem(const UObject* WorldContextObject)
{
	AInvasionGameMode* CurrentGameMode = GetInvasionGameMode(WorldContextObject);

	return CurrentGameMode ? CurrentGameMode->GetTimeDilationSystem() : nullptr;
}

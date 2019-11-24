// Fill out your copyright notice in the Description page of Project Settings.


#include "InvasionGameplayStatics.h"
#include "InvasionGameMode.h"
#include "InvasionGameState.h"
#include "InvasionPlayerState.h"
#include "InvasionPlayerController.h"

#include "Kismet/GameplayStatics.h"

#include "Characters/InvasionCharacter.h"
#include "Characters/InvasionPlayerCharacter.h"

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

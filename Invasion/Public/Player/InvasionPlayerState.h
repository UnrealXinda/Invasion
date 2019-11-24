// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "InvasionPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API AInvasionPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	AInvasionPlayerState();

	UPROPERTY()
	TSubclassOf<class UInvasionPlayerConfiguration> PlayerConfigurationClass;

	UPROPERTY()
	class UInvasionPlayerConfiguration* PlayerConfiguration;

};

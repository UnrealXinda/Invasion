// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Invasion.h"
#include "Interfaces/InvasionTick.h"
#include "Camera/PlayerCameraManager.h"
#include "Systems/TimeDilationSystem.h"
#include "InvasionPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API AInvasionPlayerCameraManager : public APlayerCameraManager, public IInvasionTick
{
	GENERATED_BODY()

	REDIRECT_TICK_FUNC()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TimeGroup)
	ETimeGroup TimeGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TimeGroup)
	float InvasionTimeDilation = 1.0F;

public:

	AInvasionPlayerCameraManager();

	UFUNCTION(BlueprintCallable)
	UCameraShake* PlayCameraShakeSingleInstance(
		TSubclassOf<class UCameraShake>   ShakeClass,
		float                             Scale = 1.f,
		enum ECameraAnimPlaySpace::Type   PlaySpace = ECameraAnimPlaySpace::CameraLocal,
		FRotator                          UserPlaySpaceRot = FRotator::ZeroRotator
	);
	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InvasionPlayerCameraManager.h"

#include "InvasionGameplayStatics.h"
#include "Camera/CameraShake.h"
#include "Camera/CameraModifier_CameraShake.h"


REDIRECT_TICK_FUNC_IMPLEMENTATION(AInvasionPlayerCameraManager)

AInvasionPlayerCameraManager::AInvasionPlayerCameraManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TimeGroup = ETimeGroup::System;
	InvasionTimeDilation = 1.0f;
}

UCameraShake* AInvasionPlayerCameraManager::PlayCameraShakeSingleInstance(
	TSubclassOf<UCameraShake>    ShakeClass,
	float                        Scale,
	ECameraAnimPlaySpace::Type   PlaySpace,
	FRotator                     UserPlaySpaceRot
)
{
	UCameraShake* ShakeInstance = nullptr;

	if (ShakeClass && CachedCameraShakeMod)
	{
		// Look for existing instance of same class
		for (UCameraShake* ShakeInst : CachedCameraShakeMod->ActiveShakes)
		{
			if (ShakeInst && (ShakeClass == ShakeInst->GetClass()))
			{
				ShakeInst->ShakeScale = Scale;
				ShakeInstance = ShakeInst;
				break;
			}
		}

		if (!ShakeInstance && CachedCameraShakeMod && (Scale > 0.0f))
		{
			ShakeInstance = CachedCameraShakeMod->AddCameraShake(ShakeClass, Scale, PlaySpace, UserPlaySpaceRot);
		}
	}

	return ShakeInstance;
}

void AInvasionPlayerCameraManager::InvasionTick_Implementation(float DeltaTime)
{
}
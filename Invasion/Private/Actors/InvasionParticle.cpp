// Fill out your copyright notice in the Description page of Project Settings.


#include "InvasionParticle.h"

#include "InvasionGameplayStatics.h"
#include "Systems/TimeDilationSystem.h"

REDIRECT_TICK_FUNC_IMPLEMENTATION(AInvasionParticle)

// Sets default values
AInvasionParticle::AInvasionParticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InvasionTimeDilation = 1.0f;
	TimeGroup = ETimeGroup::World;
}

void AInvasionParticle::InvasionTick_Implementation(float DeltaTime)
{

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/TimeDilationSystem.h"

// Sets default values
ATimeDilationSystem::ATimeDilationSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimeDilationSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimeDilationSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ATimeDilationSystem::GetTimeDilation(ETimeGroup TimeGroup)
{
	return 1.0F;
}


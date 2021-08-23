// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CoverVolume.h"
#include "Invasion.h"
#include "Components/BoxComponent.h"

// Sets default values
ACoverVolume::ACoverVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CoverComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CoverComp"));
	CoverComp->SetCanEverAffectNavigation(false);
	CoverComp->SetCollisionProfileName(CollisionProfile_Cover);
	RootComponent = CoverComp;

	LeftBlockVolumeComp = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftBlockVolumeComp"));
	LeftBlockVolumeComp->SetCanEverAffectNavigation(false);
	LeftBlockVolumeComp->SetCollisionProfileName(CollisionProfile_Cover);
	LeftBlockVolumeComp->SetupAttachment(RootComponent);

	RightBlockVolumeComp = CreateDefaultSubobject<UBoxComponent>(TEXT("RightBlockVolumeComp"));
	RightBlockVolumeComp->SetCanEverAffectNavigation(false);
	RightBlockVolumeComp->SetCollisionProfileName(CollisionProfile_Cover);
	RightBlockVolumeComp->SetupAttachment(RootComponent);

	AimingYawMin = -44.0f;
	AimingYawMax = 44.0f;
}

FVector ACoverVolume::GetLeftBlockVolumeLocation() const
{
	return LeftBlockVolumeComp->GetComponentLocation();
}

FVector ACoverVolume::GetRightBlockVolumeLocation() const
{
	return RightBlockVolumeComp->GetComponentLocation();
}

bool ACoverVolume::HasActorReachedLeftEdge(const AActor* Actor) const
{
	return LeftBlockVolumeComp->IsOverlappingActor(Actor);
}

bool ACoverVolume::HasActorReachedRightEdge(const AActor* Actor) const
{
	return RightBlockVolumeComp->IsOverlappingActor(Actor);
}

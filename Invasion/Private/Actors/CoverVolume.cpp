// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CoverVolume.h"
#include "Components/BoxComponent.h"

// Sets default values
ACoverVolume::ACoverVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CoverComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CoverComp"));
	CoverComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CoverComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CoverComp->SetCanEverAffectNavigation(false);
	RootComponent = CoverComp;

	LeftBlockVolumeComp = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftBlockVolumeComp"));
	LeftBlockVolumeComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftBlockVolumeComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	LeftBlockVolumeComp->SetCanEverAffectNavigation(false);
	LeftBlockVolumeComp->SetupAttachment(RootComponent);

	RightBlockVolumeComp = CreateDefaultSubobject<UBoxComponent>(TEXT("RightBlockVolumeComp"));
	RightBlockVolumeComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightBlockVolumeComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RightBlockVolumeComp->SetCanEverAffectNavigation(false);
	RightBlockVolumeComp->SetupAttachment(RootComponent);

	AimingYawMin = -44.0f;
	AimingYawMax = 44.0f;
}

bool ACoverVolume::HasActorReachedLeftEdge(const AActor* Actor) const
{
	return LeftBlockVolumeComp->IsOverlappingActor(Actor);
}

bool ACoverVolume::HasActorReachedRightEdge(const AActor* Actor) const
{
	return RightBlockVolumeComp->IsOverlappingActor(Actor);
}

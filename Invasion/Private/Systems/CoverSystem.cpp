// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverSystem.h"
#include "CoverVolume.h"
#include "Kismet/GameplayStatics.h"

REDIRECT_TICK_FUNC_IMPLEMENTATION(ACoverSystem)

// Sets default values
ACoverSystem::ACoverSystem()
{
	PrimaryActorTick.bCanEverTick = true;
}

TArray<ACoverVolume*> ACoverSystem::GetAvailableLowCoverVolumes(FVector TargetLoc) const
{
	TArray<ACoverVolume*> Covers;

	for (ACoverVolume* Cover : AllLowCoverVolumes)
	{
		FVector CoverLoc = Cover->GetActorLocation();
		FVector CoverForward = Cover->GetActorForwardVector();

		if (FVector::DotProduct(CoverForward, TargetLoc - CoverLoc) > 0)
		{
			Covers.Add(Cover);
		}
	}

	return Covers;
}

const TArray<ACoverVolume*>& ACoverSystem::GetAllCoverVolumes() const
{
	return AllCoverVolumes;
}

const TArray<ACoverVolume*>& ACoverSystem::GetAllLowCoverVolumes() const
{
	return AllLowCoverVolumes;
}

const TArray<ACoverVolume*>& ACoverSystem::GetAllHighCoverVolumes() const
{
	return AllHighCoverVolumes;
}

// TODO: Figure out how to get covers during level streaming
void ACoverSystem::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		TArray<AActor*> TempActorArray;
		TSubclassOf<ACoverVolume> Class;
		UGameplayStatics::GetAllActorsOfClass(World, ACoverVolume::StaticClass(), TempActorArray);

		for (AActor* Actor : TempActorArray)
		{
			if (ACoverVolume* Cover = Cast<ACoverVolume>(Actor))
			{
				AllCoverVolumes.Add(Cover);

				switch (Cover->CoverType)
				{
				case ECoverType::Low:
					AllLowCoverVolumes.Add(Cover);
					break;
				case ECoverType::High:
					AllHighCoverVolumes.Add(Cover);
					break;
				}
			}
		}
	}
}

void ACoverSystem::InvasionTick_Implementation(float DeltaTime)
{
}

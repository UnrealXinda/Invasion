// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverSystem.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ACoverSystem::ACoverSystem()
{
	PrimaryActorTick.bCanEverTick = false;
}

const TArray<class ACoverVolume*>& ACoverSystem::GetAllCoverVolumes() const
{
	return AllCoverVolumes;
}

// Called when the game starts or when spawned
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
			}
		}
	}
}

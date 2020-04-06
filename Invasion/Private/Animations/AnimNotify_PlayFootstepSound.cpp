// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayFootstepSound.h"
#include "Audio/FootstepAudioConfiguration.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UAnimNotify_PlayFootstepSound::UAnimNotify_PlayFootstepSound()
{
	VolumeMultiplier = 1.f;
	PitchMultiplier = 1.f;
}

void UAnimNotify_PlayFootstepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (FootstepAudioConfig)
	{
		if (UWorld* World = MeshComp->GetWorld())
		{
			const float kTraceLineLength = 20.0f;
			AActor* CharacterRef = MeshComp->GetOwner();
			FVector TraceStart = MeshComp->GetSocketLocation(FootSocketName);
			FVector TraceEnd = FVector(TraceStart.X, TraceStart.Y, TraceStart.Z - kTraceLineLength);

			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(CharacterRef);
			Params.bTraceComplex = true;
			Params.bReturnPhysicalMaterial = true;

			if (World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
			{
				EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
				FVector HitLoc = HitResult.Location;

				if (!TryPlaySoundAtLocation(World, SurfaceType, HitLoc))
				{
					TryPlaySoundAtLocation(World, EPhysicalSurface::SurfaceType_Default, HitLoc);
				}
			}
		}
	}
}

bool UAnimNotify_PlayFootstepSound::TryPlaySoundAtLocation(UWorld* World, EPhysicalSurface SurfaceType, FVector Location)
{
	const FFootstepAudioConfigEntry* AudioEntry = FootstepAudioConfig->Entries.FindByPredicate([SurfaceType](const FFootstepAudioConfigEntry& Entry)
	{
		return Entry.PhysicalSurface == SurfaceType;
	});

	if (AudioEntry)
	{
		UGameplayStatics::PlaySoundAtLocation(World, AudioEntry->FootstepSound, Location, VolumeMultiplier, PitchMultiplier);
	}

	return AudioEntry != nullptr;
}


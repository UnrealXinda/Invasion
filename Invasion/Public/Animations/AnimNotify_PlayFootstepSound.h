// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayFootstepSound.generated.h"

/**
 * 
 */
UCLASS(editinlinenew, Blueprintable, const, hidecategories = Object, collapsecategories, meta = (ShowWorldContextPin, DisplayName = "Invasion: Play Footstep Sound"))
class INVASION_API UAnimNotify_PlayFootstepSound : public UAnimNotify
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (DisplayName = "Footstep Audio Info"))
	class UFootstepAudioConfiguration* FootstepAudioConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	FName FootSocketName;

	// Volume Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	float VolumeMultiplier;

	// Pitch Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	float PitchMultiplier;

public:

	UAnimNotify_PlayFootstepSound();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:

	bool TryPlaySoundAtLocation(UWorld* World, EPhysicalSurface SurfaceType, FVector Location);	
};

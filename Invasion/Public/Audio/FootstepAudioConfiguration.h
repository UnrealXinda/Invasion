// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FootstepAudioConfiguration.generated.h"

USTRUCT(BlueprintType)
struct FFootstepAudioConfigEntry
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EPhysicalSurface> PhysicalSurface;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* FootstepSound;
};

/**
 * 
 */
UCLASS(BlueprintType)
class INVASION_API UFootstepAudioConfiguration : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
	TArray<FFootstepAudioConfigEntry> Entries;
};

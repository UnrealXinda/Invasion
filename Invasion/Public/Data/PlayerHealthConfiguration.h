// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerHealthConfiguration.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API UPlayerHealthConfiguration : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Time before player can self recover health when taking damage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0))
	float TimeBeforeSelfRecover;

	/** Health points recovered per second */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0))
	float SelfRecoverPerSecond;
	
};

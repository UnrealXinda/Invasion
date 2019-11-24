// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InvasionPlayerConfiguration.generated.h"

USTRUCT(BlueprintType)
struct INVASION_API FInvasionPlayerInputSetting
{
	GENERATED_BODY()

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float BaseLookUpRate;

	/** Percentage threshold below which the movement input is considered a walk. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float WalkInputThreshold;
};

/**
 * 
 */
UCLASS(Blueprintable)
class INVASION_API UInvasionPlayerConfiguration : public UObject
{
	GENERATED_BODY()

public:

	UInvasionPlayerConfiguration();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	FInvasionPlayerInputSetting InputSetting;
};

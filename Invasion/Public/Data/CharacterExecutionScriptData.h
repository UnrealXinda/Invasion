// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterExecutionScriptData.generated.h"

USTRUCT(BlueprintType)
struct FExecutionScript
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ExecutionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector VictimStartLoc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTransform CameraStartTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* ExecutionMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCameraAnim* ExecutionCameraAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveFloat* TimeDilationCurve;
};

/**
 * 
 */
UCLASS(BlueprintType)
class INVASION_API UCharacterExecutionScriptData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FExecutionScript> ExecutionScripts;
};

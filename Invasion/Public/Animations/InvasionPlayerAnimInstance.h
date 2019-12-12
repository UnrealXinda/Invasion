// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animations/InvasionAnimInstance.h"
#include "InvasionPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API UInvasionPlayerAnimInstance : public UInvasionAnimInstance
{
	GENERATED_BODY()

public:

	UInvasionPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	/** The direction between target rotation and forward vector. Used for start run/sprint animation */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = States, meta = (ClampMin = -180.0, ClampMax = 180.0))
	float OrientDirection;

	/** The angle above which the character should transit into a turn animation state for natural blending */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = States, meta = (ClampMin = 0.0, ClampMax = 180.0))
	float SuddenTurnThreshold;

	/** The flag that is driving animation state to transit into a blending state */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = States)
	bool bShouldOrient;
};

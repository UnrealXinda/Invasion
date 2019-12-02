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

};

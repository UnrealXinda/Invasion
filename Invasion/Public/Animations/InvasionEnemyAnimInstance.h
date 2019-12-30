// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animations/InvasionAnimInstance.h"
#include "InvasionEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API UInvasionEnemyAnimInstance : public UInvasionAnimInstance
{
	GENERATED_BODY()

public:

	UInvasionEnemyAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvasionEnums.h"
#include "Animation/AnimInstance.h"
#include "InvasionAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API UInvasionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UInvasionAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:

	/** The movement state of the character */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = States)
	EMoveState MoveState;

	/** The aim state of the character */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = States)
	EAimState AimState;

	/** The aim pitch of the character, used for aim offset */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = States)
	float AimPitch;

	/** The aim pitch interp speed */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = States)
	float AimPitchInterpSpeed;

	/** The cached AInvasionCharacter instance that owns this animation instance */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Character)
	class AInvasionCharacter* OwningPawn;
	
};

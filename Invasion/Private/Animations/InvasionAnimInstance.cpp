// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/InvasionAnimInstance.h"

#include "Characters/InvasionCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UInvasionAnimInstance::UInvasionAnimInstance()
{
	MoveState = EMoveState::Run;
	AimState = EAimState::Idle;
	CoverState = ECoverState::Idle;

	AimPitch = 0.0f;
	AimPitchInterpSpeed = 20.0f;
}

void UInvasionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Cache the owning pawn for use in Tick
	OwningPawn = Cast<AInvasionCharacter>(GetOwningActor());
}

void UInvasionAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (OwningPawn)
	{
		MoveState = OwningPawn->MoveState;
		AimState = OwningPawn->AimState;
		CoverState = OwningPawn->CoverState;
	}
}


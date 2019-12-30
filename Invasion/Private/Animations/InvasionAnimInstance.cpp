// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/InvasionAnimInstance.h"

#include "Characters/InvasionCharacter.h"
#include "Actors/CoverVolume.h"
#include "Components/IKComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UInvasionAnimInstance::UInvasionAnimInstance()
{
	MoveState = EMoveState::Run;
	AimState = EAimState::Idle;
	CoverState = ECoverState::Idle;

	AimPitch = 0.0f;
	AimPitchInterpSpeed = 20.0f;

	JointTargetLeft = FVector(50.000000, 217.502960, -38.598057);
	JointTargetRight = FVector(50.000000, 217.000000, 38.000000);
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
		LastMovementDir = OwningPawn->LastMovementDir;

		if (OwningPawn->CurrentCoverVolume)
		{
			CoverType = OwningPawn->CurrentCoverVolume->CoverType;
		}

		if (UIKComponent* IKComponent = OwningPawn->GetIKComponent())
		{
			if (IKComponent->bEnableIKAnimation)
			{
				LeftFootEffectorLoc = FVector(IKComponent->AnimIKLeftFootEffector, 0.0f, 0.0f);
				RightFootEffectorLoc = FVector(IKComponent->AnimIKRightFootEffector, 0.0f, 0.0f);

				LeftFootRot = IKComponent->AnimIKLeftFootRotation;
				RightFootRot = IKComponent->AnimIKRightFootRotation;
			}
		}
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/InvasionPlayerAnimInstance.h"

#include "Characters/InvasionPlayerCharacter.h"
#include "Player/InvasionPlayerController.h"

UInvasionPlayerAnimInstance::UInvasionPlayerAnimInstance()
{
}

void UInvasionPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OrientDirection = 0.0f;
	SuddenTurnThreshold = 60.0f;
}

void UInvasionPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (OwningPawn)
	{
		if (AController* Controller = OwningPawn->GetController())
		{
			// Update aim offset
			{
				FRotator ControlRot = Controller->GetControlRotation();
				FRotator ActorRot = OwningPawn->GetActorRotation();

				float TargetPitch = FRotator::NormalizeAxis(ControlRot.Pitch) - FRotator::NormalizeAxis(ActorRot.Pitch);
				AimPitch = FMath::FInterpTo(AimPitch, TargetPitch, DeltaTime, AimPitchInterpSpeed);
			}
		}

		if (AInvasionPlayerCharacter* PlayerCharacter = Cast<AInvasionPlayerCharacter>(OwningPawn))
		{
			FVector TargetMovementDir = PlayerCharacter->TargetMovementDir;
			FRotator PawnRot = OwningPawn->GetActorRotation();

			float CurrentNormalizedSpeed = PlayerCharacter->NormalizedSpeed;
			float CurrentDirection = CalculateDirection(TargetMovementDir, PawnRot);

			bool bNoInputLastFrame = (NormalizedSpeed == 0.0f);
			bool bHasInput = (CurrentNormalizedSpeed != 0.0f);

			bool bJustStartMoving = bNoInputLastFrame && bHasInput;
			bool bIsSuddenTurn = FMath::Abs(CurrentDirection - Direction) > SuddenTurnThreshold;

			bShouldOrient = bJustStartMoving || bIsSuddenTurn;

			Direction = CurrentDirection;
			NormalizedSpeed = CurrentNormalizedSpeed;
		}
	}
}

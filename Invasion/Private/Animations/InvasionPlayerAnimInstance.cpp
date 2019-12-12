// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/InvasionPlayerAnimInstance.h"

#include "Characters/InvasionPlayerCharacter.h"
#include "Player/InvasionPlayerController.h"
#include "InvasionGameplayStatics.h"

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
	EMoveState LastMoveState = MoveState;

	Super::NativeUpdateAnimation(DeltaTime);

	if (OwningPawn)
	{
		if (AInvasionPlayerController* PlayerController = UInvasionGameplayStatics::GetInvasionPlayerController(GetWorld()))
		{
			// Update aim offset
			{
				FRotator ControlRot = PlayerController->GetControlRotation();
				FRotator ActorRot = OwningPawn->GetActorRotation();

				float TargetPitch = FRotator::NormalizeAxis(ControlRot.Pitch) - FRotator::NormalizeAxis(ActorRot.Pitch);
				AimPitch = FMath::FInterpTo(AimPitch, TargetPitch, DeltaTime, AimPitchInterpSpeed);
			}
		}

		if (AInvasionPlayerCharacter* PlayerCharacter = Cast<AInvasionPlayerCharacter>(OwningPawn))
		{
			FVector TargetMovementDir = PlayerCharacter->TargetMovementDir;
			FRotator PawnRot;
			
			if (PlayerCharacter->bUseControllerRotationYaw)
			{
				PawnRot = PlayerCharacter->GetControlRotation();
				PawnRot.Roll = 0.0f;
				PawnRot.Pitch = 0.0f;
			}

			else
			{
				PawnRot = OwningPawn->GetActorRotation();
			}

			float CurrentNormalizedSpeed = PlayerCharacter->NormalizedSpeed;
			float CurrentDirection = CalculateDirection(TargetMovementDir, PawnRot);

			bool bNoInputLastFrame = (NormalizedSpeed == 0.0f);
			bool bHasInput = (CurrentNormalizedSpeed != 0.0f);

			// Start moving
			if (bNoInputLastFrame && bHasInput)
			{
				FRotator PawnRot = OwningPawn->GetActorRotation();
				OrientDirection = CurrentDirection;
				bShouldOrient = true;
			}

			// Sudden turn
			else if (FMath::Abs(CurrentDirection - Direction) > SuddenTurnThreshold)
			{
				bShouldOrient = true;
				OrientDirection = CurrentDirection;
			}

			Direction = CurrentDirection;

			NormalizedSpeed = CurrentNormalizedSpeed;
		}
	}
}

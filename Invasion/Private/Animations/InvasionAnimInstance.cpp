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

void UInvasionAnimInstance::StopAnimationOnSlot(float InBlendOutTime /*= 0.25f*/, FName SlotNodeName /*= NAME_None*/)
{
	// stop temporary montage
	// when terminate (in the Montage_Advance), we have to lose reference to the temporary montage
	if (SlotNodeName != NAME_None)
	{
		for (int32 InstanceIndex = 0; InstanceIndex < MontageInstances.Num(); InstanceIndex++)
		{
			// check if this is playing
			FAnimMontageInstance* MontageInstance = MontageInstances[InstanceIndex];
			// make sure what is active right now is transient that we created by request
			if (MontageInstance && MontageInstance->IsActive() && MontageInstance->IsPlaying())
			{
				UAnimMontage* CurMontage = MontageInstance->Montage;
				if (CurMontage && CurMontage->GetOuter() != GetTransientPackage())
				{
					// Check each track, in practice there should only be one on these
					for (int32 SlotTrackIndex = 0; SlotTrackIndex < CurMontage->SlotAnimTracks.Num(); SlotTrackIndex++)
					{
						const FSlotAnimationTrack* AnimTrack = &CurMontage->SlotAnimTracks[SlotTrackIndex];
						if (AnimTrack && AnimTrack->SlotName == SlotNodeName)
						{
							// Found it
							MontageInstance->Stop(FAlphaBlend(InBlendOutTime));
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		// Stop all
		Montage_Stop(InBlendOutTime);
	}
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


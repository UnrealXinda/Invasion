// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/InvasionPlayerAnimInstance.h"

#include "Characters/InvasionCharacter.h"
#include "Player/InvasionPlayerController.h"
#include "InvasionGameplayStatics.h"

UInvasionPlayerAnimInstance::UInvasionPlayerAnimInstance()
{
}

void UInvasionPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UInvasionPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (OwningPawn)
	{
		if (AInvasionPlayerController* PlayerController = UInvasionGameplayStatics::GetInvasionPlayerController(GetWorld()))
		{
			FRotator ControlRot = PlayerController->GetControlRotation();
			FRotator ActorRot = OwningPawn->GetActorRotation();
			
			float TargetPitch = FRotator::NormalizeAxis(ControlRot.Pitch) - FRotator::NormalizeAxis(ActorRot.Pitch);
			AimPitch = FMath::FInterpTo(AimPitch, TargetPitch, DeltaTime, AimPitchInterpSpeed);
		}
	}
}

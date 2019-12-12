// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifyState_Dashing.h"
#include "Characters/InvasionPlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_Dashing::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (AInvasionPlayerCharacter* Character = Cast<AInvasionPlayerCharacter>(MeshComp->GetOwner()))
	{
		Character->DashState = EDashState::Dashing;

	}

	if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
	{
		CachedRootMotionMode = AnimInstance->RootMotionMode;
		AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
	}
}

void UAnimNotifyState_Dashing::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AInvasionPlayerCharacter* Character = Cast<AInvasionPlayerCharacter>(MeshComp->GetOwner()))
	{
		Character->DashState = EDashState::Idle;
	}

	if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
	{
		AnimInstance->SetRootMotionMode(CachedRootMotionMode);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifyState_RootMotionRot.h"
#include "Characters/InvasionPlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_RootMotionRot::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (AInvasionPlayerCharacter* Character = Cast<AInvasionPlayerCharacter>(MeshComp->GetOwner()))
	{
		Character->bAllowRootMotionRotation = false;
	}
}

void UAnimNotifyState_RootMotionRot::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AInvasionPlayerCharacter* Character = Cast<AInvasionPlayerCharacter>(MeshComp->GetOwner()))
	{
		Character->bAllowRootMotionRotation = true;
	}
}

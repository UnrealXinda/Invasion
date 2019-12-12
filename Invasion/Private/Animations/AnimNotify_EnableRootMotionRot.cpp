// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_EnableRootMotionRot.h"
#include "Characters/InvasionPlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_EnableRootMotionRot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AInvasionPlayerCharacter* Character = Cast<AInvasionPlayerCharacter>(MeshComp->GetOwner()))
	{
		Character->bAllowRootMotionRotation = true;
	}
}

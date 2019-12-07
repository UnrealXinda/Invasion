// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifyState_Dashing.h"
#include "Characters/InvasionPlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_Dashing::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AInvasionPlayerCharacter* Character = Cast<AInvasionPlayerCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->DashState = EDashState::Dashing;
	}
}

void UAnimNotifyState_Dashing::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AInvasionPlayerCharacter* Character = Cast<AInvasionPlayerCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->DashState = EDashState::Idle;
	}
}

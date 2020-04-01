// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PauseAnimInstance.h"

void UAnimNotify_PauseAnimInstance::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	MeshComp->bPauseAnims = true;
}

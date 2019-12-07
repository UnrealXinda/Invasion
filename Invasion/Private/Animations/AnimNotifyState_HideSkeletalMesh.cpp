// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifyState_HideSkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"


void UAnimNotifyState_HideSkeletalMesh::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp)
	{
		bCachedMeshVisibility = MeshComp->bVisible;
		MeshComp->SetVisibility(false);
	}
}

void UAnimNotifyState_HideSkeletalMesh::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		MeshComp->SetVisibility(bCachedMeshVisibility);
	}
}

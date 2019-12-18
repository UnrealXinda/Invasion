// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifyState_HideSkeletalMesh.h"
#include "Characters/InvasionCharacter.h"
#include "Weapons/InvasionWeapon.h"
#include "Components/SkeletalMeshComponent.h"


void UAnimNotifyState_HideSkeletalMesh::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp)
	{
		bCachedMeshVisibility = MeshComp->bVisible;
		MeshComp->SetVisibility(false);
	}

	if (AInvasionCharacter* Character = Cast<AInvasionCharacter>(MeshComp->GetOwner()))
	{
		if (Character->CurrentWeapon)
		{
			bCachedMeshVisibility = Character->CurrentWeapon->IsWeaponVisible();
			Character->CurrentWeapon->SetWeaponVisibility(false);
		}
	}
}

void UAnimNotifyState_HideSkeletalMesh::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		MeshComp->SetVisibility(bCachedMeshVisibility);
	}

	if (AInvasionCharacter* Character = Cast<AInvasionCharacter>(MeshComp->GetOwner()))
	{
		if (Character->CurrentWeapon)
		{
			Character->CurrentWeapon->SetWeaponVisibility(bCachedMeshVisibility);
		}
	}
}

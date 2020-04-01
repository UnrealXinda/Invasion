// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayCameraShake.h"
#include "Camera/CameraShake.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_PlayCameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (CameraShakeClass)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			if (UWorld* World = Owner->GetWorld())
			{
				if (APlayerCameraManager* Manager = UGameplayStatics::GetPlayerCameraManager(World, 0))
				{
					Manager->PlayCameraShake(CameraShakeClass);
				}
			}
		}
	}
}

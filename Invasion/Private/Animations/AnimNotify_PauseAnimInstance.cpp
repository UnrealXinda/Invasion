// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PauseAnimInstance.h"

void UAnimNotify_PauseAnimInstance::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
#ifdef UE_BUILD_DEBUG
	if (UWorld* World = MeshComp->GetWorld())
	{
		switch (World->WorldType)
		{
		case EWorldType::Game:
		case EWorldType::PIE:
#endif
			MeshComp->bPauseAnims = true;

#ifdef UE_BUILD_DEBUG
			break;
		}
	}
#endif
}

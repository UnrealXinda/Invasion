// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayCameraShake.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Invasion: Play Camera Shake"))
class INVASION_API UAnimNotify_PlayCameraShake : public UAnimNotify
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (DisplayName = "Camera Shake"))
	TSubclassOf<class UCameraShake> CameraShakeClass;

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;	
};

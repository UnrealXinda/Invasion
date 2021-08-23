// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_EnableRootMotionRot.generated.h"

/**
 * 
 */
UCLASS(editinlinenew, Blueprintable, const, hidecategories = Object, collapsecategories, meta = (ShowWorldContextPin, DisplayName = "Invasion: Enable Root Motion Rotation"))
class INVASION_API UAnimNotify_EnableRootMotionRot : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

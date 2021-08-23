// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PauseAnimInstance.generated.h"

/**
 * 
 */
UCLASS(editinlinenew, Blueprintable, const, hidecategories = Object, collapsecategories, meta = (ShowWorldContextPin, DisplayName = "Invasion: Pause Anim Instance"))
class INVASION_API UAnimNotify_PauseAnimInstance : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

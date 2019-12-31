// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvasionEnums.h"
#include "Animation/AnimInstance.h"
#include "InvasionAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API UInvasionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	/** The normalized movement speed */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = States, meta = (ClampMin = 0.0, ClampMax = 1.0))
	float NormalizedSpeed;

	/** The angle between movement direction and speed vector */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = States, meta = (ClampMin = -180.0, ClampMax = 180.0))
	float Direction;

	/** The movement state of the character */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = States)
	EMoveState MoveState;

	/** The aim state of the character */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = States)
	EAimState AimState;

	/** The cover state of the character */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = States)
	ECoverState CoverState;

	/** The cover type of the cover the character is taking, if any */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = States)
	ECoverType CoverType;

	/** The last movement direction of the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = States)
	EMoveDirection LastMovementDir;

public:

	UInvasionAnimInstance();

	/** Stops currently playing slot animation slot or all*/
	// This is a fix for UAnimInstance's StopSlotAnimation function.
	// Instead of editing the engine code, I simply re-implement the function here since all
	// variables are public.
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StopAnimationOnSlot(float InBlendOutTime = 0.25f, FName SlotNodeName = NAME_None);

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	FVector LeftFootEffectorLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	FVector RightFootEffectorLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	FVector JointTargetLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	FVector JointTargetRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	FRotator LeftFootRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	FRotator RightFootRot;

	/** The aim pitch of the character, used for aim offset */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = States)
	float AimPitch;

	/** The aim pitch interp speed */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = States)
	float AimPitchInterpSpeed;

	/** The cached AInvasionCharacter instance that owns this animation instance */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Character)
	class AInvasionCharacter* OwningPawn;
	
};

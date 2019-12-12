// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvasionCharacter.h"
#include "InvasionPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API AInvasionPlayerCharacter : public AInvasionCharacter
{
	GENERATED_BODY()

public:	

	/** The dash state of the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = States)
	EDashState DashState;
	
public:

	AInvasionPlayerCharacter();	

	/** The normalized speed for root motion */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Movement)
	float NormalizedSpeed;

	/** The target movement direction for root motion blending */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Movement)
	FVector TargetMovementDir;

	/** If true, rotation should be handled by root motion other than controller */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Movement)
	bool bAllowRootMotionRotation;

	/** The rotation interpolation speed used for controlling character facing direction while running */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float RunRotationInterpSpeed;

	/** The rotation interpolation speed used for controlling character facing direction while sprinting */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float SprintRotationInterpSpeed;

	virtual FVector GetPawnViewLocation() const override;

	virtual void InvasionTick_Implementation(float DeltaTime) override;

protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** The animation montage used for dashing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	class UAnimMontage* DashMontage;

public:

	UFUNCTION(BlueprintCallable)
	virtual bool CanDash() const;

	virtual bool CanMove() const override;

	virtual bool CanSprint() const override;

	virtual bool CanAim() const override;

	virtual bool CanFire() const override;

	virtual void MoveCharacter(FVector WorldDirection, float ScaleValue /* = 1.0F */) override;

	void Dash(FRotator Direction);

protected:

};

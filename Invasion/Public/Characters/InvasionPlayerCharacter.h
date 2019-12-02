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

	AInvasionPlayerCharacter();

	virtual FVector GetPawnViewLocation() const override;

	virtual void InvasionTick_Implementation(float DeltaTime) override;

protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

protected:

	void TickMovementSpeed();

};

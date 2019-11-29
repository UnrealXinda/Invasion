// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Invasion.h"
#include "InvasionEnums.h"
#include "Interfaces/InvasionTick.h"
#include "GameFramework/PlayerController.h"
#include "Systems/TimeDilationSystem.h"
#include "InvasionPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API AInvasionPlayerController : public APlayerController, public IInvasionTick
{
	GENERATED_BODY()

	REDIRECT_TICK_FUNC()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TimeGroup)
	ETimeGroup TimeGroup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TimeGroup)
	float InvasionTimeDilation = 1.0F;

public:

	AInvasionPlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

#pragma region Movement

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Called from Tick to actually process movement */
	void TickCharacterMovement();

	/** Called via input to turn at a given rate. */
	void Turn(float Rate);

	/** Called via input to turn look up/down at a given rate. */
	void LookUp(float Rate);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Called for dashing in current input direction */
	void OnPressDash();

	/** Called when sprint button pressed down */
	void OnPressSprint();

	/** Called when sprint button released */
	void OnReleaseSprint();

	/** Called when aim button pressed down */
	void OnPressAim();

	/** Called when aim button released */
	void OnReleaseAim();

	/** Called when fire button pressed down */
	void OnPressFire();

	/** Called when fire button released */
	void OnReleaseFire();

	/** Get the last input vector's rotation and map to world space */
	UFUNCTION(BlueprintPure)
	FRotator GetWorldInputRotation() const;

	/** Get the last input vector (normalize) and map to world space */
	UFUNCTION(BlueprintPure)
	FVector GetWorldInputVector() const;

	/** Returns true if the given action key is pressed down */
	UFUNCTION(BlueprintCallable)
	bool IsActionKeyDown(FName ActionName);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	class AInvasionPlayerCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	class AInvasionPlayerState* InvasionPlayerState;

	/** Local cache of the last input vector */
	FVector LastMovementInputVector;
	
};

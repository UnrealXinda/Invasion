// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Invasion.h"
#include "InvasionEnums.h"
#include "Interfaces/InvasionTick.h"
#include "GameFramework/Character.h"
#include "InvasionCharacter.generated.h"

UCLASS()
class INVASION_API AInvasionCharacter : public ACharacter, public IInvasionTick
{
	GENERATED_BODY()

	REDIRECT_TICK_FUNC()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TimeGroup)
	ETimeGroup TimeGroup;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TimeGroup)
	float InvasionTimeDilation = 1.0f;

public:

	/** The movement speed for walking */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float MaxWalkSpeed;

	/** The movement speed for running */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float MaxRunSpeed;

	/** The movement speed for sprinting */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float MaxSprintSpeed;

	/** The movement state of the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = States)
	EMoveState MoveState;

	/** The aim state of the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = States)
	EAimState AimState;

	/** The current weapon used by the character */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Weapon)
	class AInvasionWeapon* CurrentWeapon;

	/** Initial class that the player owns when begin play */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<class AInvasionWeapon> StarterWeaponClass;

	/** Socket name that the weapon is attached to the character mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName WeaponSocketName;

public:

	// Sets default values for this character's properties
	AInvasionCharacter();

	UFUNCTION(BlueprintPure)
	virtual bool CanMove() const;

	UFUNCTION(BlueprintPure)
	virtual bool CanSprint() const;

	UFUNCTION(BlueprintPure)
	virtual bool CanAim() const;

	UFUNCTION(BlueprintPure)
	virtual bool CanFire() const;

	void MoveCharacter(FVector WorldDirection, float ScaleValue = 1.0F);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

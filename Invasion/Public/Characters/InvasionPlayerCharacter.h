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

	/** The dash state of the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = States)
	EDashState DashState;

	/** The execute state of the character */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = States)
	EExecuteState ExecuteState;

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

public:

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetExecutableCharacters() const;

	UFUNCTION(BlueprintNativeEvent)
	void ExecuteCharacter(AInvasionCharacter* Victim);

	UFUNCTION(BlueprintCallable)
	virtual bool CanDash() const;	

	UFUNCTION(BlueprintPure)
	virtual bool CanExecute() const;

	virtual bool CanMove() const override;

	virtual bool CanSprint() const override;

	virtual bool CanAim() const override;

	virtual bool CanFire() const override;

	virtual bool CanTakeCover() const override;

	virtual void StartFire() override;

	virtual void StopFire() override;

	virtual void MoveCharacter(FVector WorldDirection, float ScaleValue /* = 1.0F */) override;

	virtual bool EquipWeapon(class AInvasionWeapon* Weapon) override;

	virtual bool UnequipWeapon(class AInvasionWeapon* Weapon) override;

	virtual FVector GetPawnViewLocation() const override;

	virtual void InvasionTick_Implementation(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor = NULL) const override;

	void Dash(FRotator Direction);

	void Heal(float RecoverAmount);

protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Sphere component used to detect overlapping enemy characters for execution */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComp;

	/** Energy component for tracking current available energy */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class UEnergyComponent* EnergyComp;

	/** The animation montage used for dashing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	class UAnimMontage* DashMontage;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = States)
	bool bCanSelfRecover;

	FTimerHandle ResetSelfRecoverTimerHandle;

protected:

	UFUNCTION()
	void OnWeaponFire(class AInvasionWeapon* Weapon, class AController* InstigatedBy);

	UFUNCTION()
	void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor*              OtherActor,
		UPrimitiveComponent* OtherComp,
		int32                OtherBodyIndex,
		bool                 bFromSweep,
		const FHitResult&    SweepResult);

	UFUNCTION()
	void EnableSelfRecover();

	UFUNCTION()
	void OnPlayerTakeAnyDamage(
		AActor*                  DamagedActor,
		float                    Damage,
		const class UDamageType* DamageType,
		class AController*       InstigatedBy,
		AActor*                  DamageCauser);

	virtual void OnCharacterDeath(
		class UHealthComponent*  HealthComponent,
		float                    LastDamage,
		const class UDamageType* DamageType,
		class AController*       InstigatedBy,
		AActor*                  DamageCauser
	) override;

	virtual void BeginPlay() override;

	void RecoverHealth(float DeltaSeconds);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/InvasionWeapon.h"
#include "InvasionRifle.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API AInvasionRifle : public AInvasionWeapon
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* WeaponSkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<class UCameraShake> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	class UAnimationAsset* FireAnimation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	/** Used for simulating recoil for AI characters when using this rifle. 
	/** Bullet spread in degrees */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UCurveFloat* RandomConeHalfDegreesPerDistance;

public:

	AInvasionRifle();

	UFUNCTION(BlueprintPure)
	float GetDamageAmountFromSurfaceType(EPhysicalSurface SurfaceType) const;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Fire() override;

	virtual void StartFire() override;

	virtual void StopFire() override;

	virtual void SetWeaponVisibility(bool bVisible) override;

	virtual bool IsWeaponVisible() const override;

	void AddRecoilToController(class APlayerController* Controller) const;

protected:

	/** The last game time when fired */
	float LastFireTime;

	/** Timer handle for scheduling automatic fire */
	FTimerHandle ShotIntervalTimerHandle;

	/** Random stream for RNG */
	FRandomStream RandomStream;

protected:

	void PlayFireEffects();
	void PlayFireAnimation();
	void PlayMuzzleEffect();
	void PlayImpactEffect(FVector Location, FRotator Rotation, EPhysicalSurface SurfaceType);
	void PlayTracerEffect(FVector TargetPoint);
	void PlayCameraShakeEffect();
	void BroadcastOnWeaponFire();

	void GetWeaponTraceForPlayer(FVector& TraceStart, FVector& TraceEnd, APawn* OwningPawn) const;
	void GetWeaponTraceForAI(FVector& TraceStart, FVector& TraceEnd, APawn* OwningPawn) const;
};
